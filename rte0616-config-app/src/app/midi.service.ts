import { Inject, Injectable } from '@angular/core';
import { MIDI_INPUTS, MIDI_MESSAGES, MIDI_OUTPUT, MIDI_OUTPUTS } from '@ng-web-apis/midi';
import { Observable, Subject, firstValueFrom } from 'rxjs';

const SYSEX_PREFIX = 0xF0
const SYSEX_SUFFIX = 0xF7
const MANU_ID = 0x31;
const PRODUCT_ID = [0x06, 0x16];

enum SYSEX_CMD {
  CMD_CONFIGURATION = 1,
	CMD_CALIBRATION,
	CMD_CHANGE_MODE
}

enum SYSEX_SUB_CMD {
	SUBCMD_GET = 1,
	SUBCMD_SET
}

enum SYSEX_MODE {
	MODE_STANDARD = 1,
	MODE_MEASURE,
}

export class DeviceMIDIConfig {
  constructor(public channel = 0, public octave = 0)
  {}
}

function decodeDeviceMidiConfig(data: Uint8Array): DeviceMIDIConfig {
  let chan = data!.at(0)!
  let oct = data!.at(1)!

  // Clamp
  if (chan < 1) chan = 0
  if (chan > 16) chan = 16
  if (oct < 1) oct = 1
  if (chan > 8) oct = 8

  return new DeviceMIDIConfig(chan, oct)
}

function isEqualBytes(ba1: Uint8Array, ba2: Uint8Array): boolean {
  if (ba1.length !== ba2.length)
      return false;

  for (let i = 0; i < ba1.length; i++) {
    if (ba1[i] !== ba2[i])
      return false;
  }

  return true;
}

function promiseWithTimeout<T>(
  promise: Promise<T>,
  ms: number,
  timeoutError = new Error('Time out')
): Promise<T> {
  // create a promise that rejects in milliseconds
  const timeout = new Promise<never>((_, reject) => {
    setTimeout(() => {
      reject(timeoutError);
    }, ms);
  });

  // returns a race between timeout and the passed promise
  return Promise.race<T>([promise, timeout]);
}


@Injectable({
  providedIn: 'root'
})
export class MidiService implements EventListenerObject {
  // Ugly.
  output: MIDIOutput|null = null;
  input: MIDIInput|null = null;

  fwVersion$ = new Subject<Uint8Array>
  midiConfig$ = new Subject<DeviceMIDIConfig>

  constructor(
    @Inject(MIDI_INPUTS) private inputs$: Observable<MIDIInput[]>,
    @Inject(MIDI_OUTPUTS) private outputs$: Observable<MIDIOutput[]>,
    // private readonly messages$: Observable<MIDIMessageEvent>
  )
  {
    // messages$.subscribe(msg => console.log(msg))
  }

  selectDevice(outputName: string) {
    // Horrible.
    let outClientName = outputName.split(':')[0]
    var inClientName = (outClientName == 'receivemidi')? 'sendmidi' : outClientName

    this.outputs$.subscribe({next: outputs => { this.output = outputs.filter(o => (o.name == outputName))[0]; console.log(this.output)}})
    this.inputs$.subscribe({next: inputs => { console.log(inputs); this.input = inputs.filter(i => i.name?.startsWith(inClientName))[0]; console.log(this.input); this.input?.addEventListener('midimessage', this)}})
  }

  isDeviceSelected() {
    return (this.input && this.output);
  }

  handleEvent(ev: MIDIMessageEvent) {
    console.log(ev)
    if ((ev.data!.at(0) == SYSEX_PREFIX) && (ev.data!.at(-1) == SYSEX_SUFFIX)) {
      console.log('SYSEX: ' + ev.data!.slice(1,-1));
      if (isEqualBytes(ev.data!.slice(1,5), Uint8Array.from([0x7E, 0x01, 0x06, 0x02])))
      {
        if (isEqualBytes(ev.data!.slice(5,11), Uint8Array.from([0x00, MANU_ID, 0x00, MANU_ID, ...PRODUCT_ID ])))
        {
          this.fwVersion$.next(ev.data!.slice(11,14))
        }
      }
      else {
        if (isEqualBytes(ev.data!.slice(1,4), Uint8Array.from([MANU_ID, ...PRODUCT_ID]))) {
          switch (ev.data!.at(4)) {
            case SYSEX_CMD.CMD_CONFIGURATION:
              if (ev.data!.at(5) == SYSEX_SUB_CMD.SUBCMD_GET)
                this.midiConfig$.next(decodeDeviceMidiConfig(ev.data!.slice(6, 8)));
              break;
            case SYSEX_CMD.CMD_CALIBRATION:
            case SYSEX_CMD.CMD_CHANGE_MODE:
            default:
              console.log('Unknown command ' + ev.data!.at(4))
              break;
          }
        }
      }
    }
  }

  async sendIdentRequest(): Promise<Uint8Array> {
    if (this.input && this.output) {
      console.log('Sending Ident Sysex request to ' + this.output.name)
      this.output.send([SYSEX_PREFIX, 0x7E, 0x7F, 0x06, 0x01, SYSEX_SUFFIX])

      return promiseWithTimeout(new Promise(async resolve => {
        resolve(await firstValueFrom(this.fwVersion$))
      }), 2000)
    }
    else {
      return new Promise((resolve, reject) => reject(this.getInputOutputError()))
    }
  }

  async sendGetConfigurationRequest() {
    if (this.input && this.output) {
      console.log('Sending Get Config Sysex request to ' + this.output.name)
      this.output.send([SYSEX_PREFIX, MANU_ID, ...PRODUCT_ID, SYSEX_CMD.CMD_CONFIGURATION, SYSEX_SUB_CMD.SUBCMD_GET, SYSEX_SUFFIX])
    }
    else {
      this.getInputOutputError()
    }
  }

  async sendSetConfigurationRequest(midiConfig: DeviceMIDIConfig) {
    if (this.input && this.output) {
      console.log('Sending Get Config Sysex request to ' + this.output.name)
      this.output.send([SYSEX_PREFIX, MANU_ID, ...PRODUCT_ID, SYSEX_CMD.CMD_CONFIGURATION, SYSEX_SUB_CMD.SUBCMD_SET, midiConfig.channel, midiConfig.octave, SYSEX_SUFFIX])
    }
    else {
      this.getInputOutputError()
    }
  }

  async sendStandardModeRequest() {
    if (this.input && this.output) {
      console.log('Sending Change to standard mode Sysex request to ' + this.output.name)
      this.output.send([SYSEX_PREFIX, MANU_ID, ...PRODUCT_ID, SYSEX_CMD.CMD_CHANGE_MODE, SYSEX_MODE.MODE_STANDARD, SYSEX_SUFFIX])
    }
    else {
      this.getInputOutputError()
    }
  }

  getInputOutputError(): Error {
    let error = new Error()
    if (!this.input)  error.message = 'Invalid input device'
    else if (!this.output) error.message = 'No output device matching ' + this.input!.name
    console.log(error)
    return error;
  }
}
