import { Inject, Injectable, inject } from '@angular/core';
import { ActivatedRouteSnapshot, CanActivateFn, Router, RouterStateSnapshot, UrlTree } from '@angular/router';
import { MIDI_INPUTS, MIDI_MESSAGES, MIDI_OUTPUT, MIDI_OUTPUTS } from '@ng-web-apis/midi';
import { Observable, Subject, firstValueFrom, throttleTime } from 'rxjs';

const SYSEX_PREFIX = 0xF0
const SYSEX_SUFFIX = 0xF7
const MANU_ID = 0x31
const PRODUCT_ID = [0x06, 0x16]

const MIDI_MIN_CHAN = 1
const MIDI_MAX_CHAN = 16
const MIDI_MAX_BYTE = 0x7F

const PDLBRD_NB_SEGMENTS = 4
const PDLBRD_NB_OUT_PINS = 8
const PDLBRD_MIN_OCTAVE = 1
const PDLBRD_MAX_OCTAVE = 6
const PDLBRD_MIN_PIN_VAL = 0
const PDLBRD_MAX_PIN_VAL = 1023
const PDLBRD_MIN_MARGIN = 1
const PDLBRD_MAX_MARGIN = 1023

export enum PDLBRD_MODE {
  MODE_MONODIC_UP = 0,
  MODE_POLYPHONIC,
}

enum SYSEX_CMD {
  CMD_CONFIGURATION = 1,
	CMD_CALIBRATION,
  CMD_MEASURES,
	CMD_CHANGE_MODE
}

enum SYSEX_SUB_CMD {
  SUBCMD_NO_SUBCMD = 0,
	SUBCMD_GET,
	SUBCMD_SET,
	SUBCMD_STORE
}

enum SYSEX_MODE {
	MODE_STANDARD = 1,
	MODE_MEASURE
}

export class FirmwareVersion {
  constructor(public major = 0, public minor = 0, public patch = 0)
  {}

  toString() {
    return this.major + '.' + this.minor + '.' + this.patch;
  }
}

export class DeviceMIDIConfig {
  constructor(public channel = MIDI_MIN_CHAN, public octave = PDLBRD_MIN_OCTAVE, public keyboardMode = PDLBRD_MODE.MODE_MONODIC_UP)
  {}
}

export class DeviceCalibration {
  constructor(public margin = PDLBRD_MIN_MARGIN, public vSeg = Array<number>(PDLBRD_NB_SEGMENTS).fill(PDLBRD_MIN_PIN_VAL))
  {}
}

export class DeviceMeasures {
  constructor(public v = Array<number>(PDLBRD_NB_OUT_PINS).fill(PDLBRD_MIN_PIN_VAL))
  {}
}

function toMIDILsbMsb(val: number): [number, number] {
  return [(val >> 7) & MIDI_MAX_BYTE, val & MIDI_MAX_BYTE];
}

function fromMIDILsbMsb(val: [number, number]): number {
  return ((val[0] & MIDI_MAX_BYTE) << 7) + (val[1] & MIDI_MAX_BYTE);
}

function decodeFirmwareVersion(data: Uint8Array): FirmwareVersion {
  let major = data!.at(0)
  let minor = data!.at(1)
  let patch = fromMIDILsbMsb([data!.at(2)!, data!.at(3)!])

  return new FirmwareVersion(major, minor, patch)
}

function decodeDeviceMidiConfig(data: Uint8Array): DeviceMIDIConfig {
  let chan = data!.at(0)!
  let oct = data!.at(1)!
  let mode = data!.at(2)!

  // Clamp
  if (chan < MIDI_MIN_CHAN) chan = MIDI_MIN_CHAN
  if (chan > MIDI_MAX_CHAN) chan = MIDI_MAX_CHAN
  if (oct < PDLBRD_MIN_OCTAVE) oct = PDLBRD_MIN_OCTAVE
  if (oct > PDLBRD_MAX_OCTAVE) oct = PDLBRD_MAX_OCTAVE
  if ((mode < 0) || (mode > PDLBRD_MODE.MODE_POLYPHONIC)) mode = PDLBRD_MODE.MODE_MONODIC_UP;

  return new DeviceMIDIConfig(chan, oct, mode);
}

function encodeDeviceMidiConfig(midiConfig: DeviceMIDIConfig): Array<number> {
  return [SYSEX_PREFIX, MANU_ID, ...PRODUCT_ID, SYSEX_CMD.CMD_CONFIGURATION, SYSEX_SUB_CMD.SUBCMD_SET,
    midiConfig.channel, midiConfig.octave, midiConfig.keyboardMode,
    SYSEX_SUFFIX];
}

function decodeDeviceCalibration(data: Uint8Array): DeviceCalibration {
  let margin = fromMIDILsbMsb([data!.at(0)!, data!.at(1)!])
  let vSeg1  = fromMIDILsbMsb([data!.at(2)!, data!.at(3)!])
  let vSeg2  = fromMIDILsbMsb([data!.at(4)!, data!.at(5)!])
  let vSeg3  = fromMIDILsbMsb([data!.at(6)!, data!.at(7)!])
  let vSeg4  = fromMIDILsbMsb([data!.at(8)!, data!.at(9)!])

  return new DeviceCalibration(margin, [vSeg1, vSeg2, vSeg3, vSeg4]);
}

function encodeDeviceCalibration(calibration: DeviceCalibration): Array<number> {
  let margin = toMIDILsbMsb(calibration.margin)
  let vSeg1 = toMIDILsbMsb(calibration.vSeg[0])
  let vSeg2 = toMIDILsbMsb(calibration.vSeg[1])
  let vSeg3 = toMIDILsbMsb(calibration.vSeg[2])
  let vSeg4 = toMIDILsbMsb(calibration.vSeg[3])

  return [SYSEX_PREFIX, MANU_ID, ...PRODUCT_ID,
    SYSEX_CMD.CMD_CALIBRATION, SYSEX_SUB_CMD.SUBCMD_SET,
    ...margin, ...vSeg1, ...vSeg2, ...vSeg3, ...vSeg4,
    SYSEX_SUFFIX];
}

function decodeDeviceMeasures(data: Uint8Array): DeviceMeasures {
  let v1 = fromMIDILsbMsb([data!.at(0)!, data!.at(1)!])
  let v2 = fromMIDILsbMsb([data!.at(2)!, data!.at(3)!])
  let v3 = fromMIDILsbMsb([data!.at(4)!, data!.at(5)!])
  let v4 = fromMIDILsbMsb([data!.at(6)!, data!.at(7)!])
  let v5 = fromMIDILsbMsb([data!.at(8)!, data!.at(9)!])
  let v6 = fromMIDILsbMsb([data!.at(10)!, data!.at(11)!])
  let v7 = fromMIDILsbMsb([data!.at(12)!, data!.at(13)!])
  let v8 = fromMIDILsbMsb([data!.at(14)!, data!.at(15)!])

  return new DeviceMeasures([v1, v2, v3, v4, v5, v6, v7, v8]);
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
  output: MIDIOutput|undefined = undefined;
  input: MIDIInput|undefined = undefined;

  fwVersion$ = new Subject<FirmwareVersion>
  midiConfig$ = new Subject<DeviceMIDIConfig>
  calibration$ = new Subject<DeviceCalibration>
  measures$ = new Observable<DeviceMeasures>
  private m$ = new Subject<DeviceMeasures>

  constructor(
    @Inject(MIDI_INPUTS) private inputs$: Observable<MIDIInput[]>,
    @Inject(MIDI_OUTPUTS) private outputs$: Observable<MIDIOutput[]>,
    // private readonly messages$: Observable<MIDIMessageEvent>
  )
  {
    // messages$.subscribe(msg => console.log(msg))
    this.measures$ = this.m$.pipe(throttleTime(200))
  }

  selectDevice(outputName: string) {
    // Horrible.
    let outClientName = outputName.split(':')[0]
    var inClientName = (outClientName == 'receivemidi')? 'sendmidi' : outClientName

    this.outputs$.subscribe({next: outputs => { this.output = outputs.filter(o => (o.name == outputName))[0]; console.log(this.output)}})
    this.inputs$.subscribe({next: inputs => { console.log(inputs); this.input = inputs.filter(i => i.name?.startsWith(inClientName))[0]; console.log(this.input); this.input?.addEventListener('midimessage', this)}})
  }

  isDeviceSelected(): boolean {
    return ((this.input!=undefined) && (this.output!=undefined))
  }

  handleEvent(ev: MIDIMessageEvent) {
    console.log(ev)
    if ((ev.data!.at(0) == SYSEX_PREFIX) && (ev.data!.at(-1) == SYSEX_SUFFIX)) {
      console.log('SYSEX: ' + ev.data!.slice(1,-1));
      if (isEqualBytes(ev.data!.slice(1,5), Uint8Array.from([0x7E, 0x01, 0x06, 0x02])))
      {
        if (isEqualBytes(ev.data!.slice(5,10), Uint8Array.from([MANU_ID, 0x00, MANU_ID, ...PRODUCT_ID ])))
        {
          this.fwVersion$.next(decodeFirmwareVersion(ev.data!.slice(10,14)))
        }
      }
      else {
        if (isEqualBytes(ev.data!.slice(1,4), Uint8Array.from([MANU_ID, ...PRODUCT_ID]))) {
          switch (ev.data!.at(4)) {
            case SYSEX_CMD.CMD_CONFIGURATION:
                console.log(ev.data!.slice(6, 9))
                this.midiConfig$.next(decodeDeviceMidiConfig(ev.data!.slice(6, 9)))
              break;
              case SYSEX_CMD.CMD_CALIBRATION:
                  this.calibration$.next(decodeDeviceCalibration(ev.data!.slice(6, 16)))
                break;
              case SYSEX_CMD.CMD_MEASURES:
                  this.m$.next(decodeDeviceMeasures(ev.data!.slice(6, 22)))
                break;
            case SYSEX_CMD.CMD_CHANGE_MODE:
            default:
              console.log('Unknown command ' + ev.data!.at(4))
              break;
          }
        }
      }
    }
  }

  async sendIdentRequest(): Promise<FirmwareVersion> {
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

  async sendStandardModeRequest() {
    if (this.input && this.output) {
      console.log('Sending Change to standard mode Sysex request to ' + this.output.name)
      this.output.send([SYSEX_PREFIX, MANU_ID, ...PRODUCT_ID, SYSEX_CMD.CMD_CHANGE_MODE, SYSEX_MODE.MODE_STANDARD, SYSEX_SUFFIX])
    }
    else {
      this.getInputOutputError()
    }
  }

  async sendMeasureModeRequest() {
    if (this.input && this.output) {
      console.log('Sending Change to measure mode Sysex request to ' + this.output.name)
      this.output.send([SYSEX_PREFIX, MANU_ID, ...PRODUCT_ID, SYSEX_CMD.CMD_CHANGE_MODE, SYSEX_MODE.MODE_MEASURE, SYSEX_SUFFIX])
    }
    else {
      this.getInputOutputError()
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
      console.log('Sending Set Config Sysex request to ' + this.output.name)
      this.output.send(encodeDeviceMidiConfig(midiConfig))
    }
    else {
      this.getInputOutputError()
    }
  }

  async sendStoreConfigurationRequest() {
    if (this.input && this.output) {
      console.log('Sending Store Config Sysex request to ' + this.output.name)
      this.output.send([SYSEX_PREFIX, MANU_ID, ...PRODUCT_ID, SYSEX_CMD.CMD_CONFIGURATION, SYSEX_SUB_CMD.SUBCMD_STORE, SYSEX_SUFFIX])
    }
    else {
      this.getInputOutputError()
    }
  }

  async sendGetCalibrationRequest() {
    if (this.input && this.output) {
      console.log('Sending Get Calibration Sysex request to ' + this.output.name)
      this.output.send([SYSEX_PREFIX, MANU_ID, ...PRODUCT_ID, SYSEX_CMD.CMD_CALIBRATION, SYSEX_SUB_CMD.SUBCMD_GET, SYSEX_SUFFIX])
    }
    else {
      this.getInputOutputError()
    }
  }

  async sendSetCalibrationRequest(calibration: DeviceCalibration) {
    if (this.input && this.output) {
      console.log('Sending Set Calibration Sysex request to ' + this.output.name)
      this.output.send(encodeDeviceCalibration(calibration))
    }
    else {
      this.getInputOutputError()
    }
  }

  async sendStoreCalibrationRequest() {
    if (this.input && this.output) {
      console.log('Sending Store Calibration Sysex request to ' + this.output.name)
      this.output.send([SYSEX_PREFIX, MANU_ID, ...PRODUCT_ID, SYSEX_CMD.CMD_CALIBRATION, SYSEX_SUB_CMD.SUBCMD_STORE, SYSEX_SUFFIX])
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

export const DeviceSelectedGuard: CanActivateFn = (next: ActivatedRouteSnapshot, state: RouterStateSnapshot): boolean|UrlTree => {
  if (inject(MidiService).isDeviceSelected()) {
    return true;
  }
  else {
    return inject(Router).parseUrl('devices');
  }
}