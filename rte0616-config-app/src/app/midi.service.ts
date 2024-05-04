import { Inject, Injectable } from '@angular/core';
import { MIDI_INPUTS, MIDI_MESSAGES, MIDI_OUTPUT, MIDI_OUTPUTS } from '@ng-web-apis/midi';
import { Observable, Subject, firstValueFrom } from 'rxjs';

const MANU_ID = 0x31;
const PRODUCT_ID = [0x06, 0x16];

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
  timeoutError = new Error('Promise timed out')
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
    var outputName = outputName.split(':')[0]
    var inputName = (outputName == 'receivemidi')? 'sendmidi' : outputName

    this.outputs$.subscribe({next: outputs => { this.output = outputs.filter(o => o.name?.startsWith(outputName))[0]; console.log(this.output)}})
    this.inputs$.subscribe({next: inputs => { console.log(inputs); this.input = inputs.filter(i => i.name?.startsWith(inputName))[0]; console.log(this.input); this.input?.addEventListener('midimessage', this)}})
  }

  handleEvent(ev: MIDIMessageEvent) {
    console.log(ev)
    if ((ev.data!.at(0) == 0xF0) && (ev.data!.at(-1) == 0xF7)) {
      console.log('SYSEX: ' + ev.data!.slice(1,-1));
      if (isEqualBytes(ev.data!.slice(1,5), Uint8Array.from([0x7E, 0x01, 0x06, 0x02])))
      {
        if (isEqualBytes(ev.data!.slice(5,11), Uint8Array.from([0x00, MANU_ID, 0x00, MANU_ID, ...PRODUCT_ID ])))
        {
          this.fwVersion$.next(ev.data!.slice(11,14))
        }
      }
    }
  }

  async sendIdentSysex(): Promise<Uint8Array> {
    if (this.input && this.output) {
      console.log('Sending Ident Sysex to ' + this.output.name)
      this.output.send([0xF0, 0x7E, 0x7F, 0x06, 0x01, 0xF7])

      return promiseWithTimeout(new Promise(async resolve => {
        resolve(await firstValueFrom(this.fwVersion$))
      }), 2000)
    }
    else {
      console.log('No output or input')
      return new Promise((resolve, reject) => reject('No output or input'))
    }
  }

}
