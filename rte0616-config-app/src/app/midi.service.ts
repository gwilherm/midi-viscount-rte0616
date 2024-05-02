import { Directive, Inject, Injectable, input } from '@angular/core';
import { MIDI_INPUTS, MIDI_MESSAGES, MIDI_OUTPUT, MIDI_OUTPUTS } from '@ng-web-apis/midi';
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class MidiService implements EventListenerObject {
  // Ugly.
  output: MIDIOutput|null = null;
  input: MIDIInput|null = null;

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
    outputName = outputName.split(':')[0]
    this.outputs$.subscribe({next: outputs => { this.output = outputs.filter(o => o.name?.startsWith(outputName))[0]; console.log(this.output)}})
    this.inputs$.subscribe({next: inputs => { console.log(inputs); this.input = inputs.filter(i => i.name?.startsWith(outputName))[0]; console.log(this.input); this.input?.addEventListener('midimessage', this)}})
  }

  handleEvent(ev: Event) {
    console.log(ev)
  }

  sendIdentSysex() {
    if (this.input && this.output) {
      console.log('Sending Ident Sysex to ' + this.output.name)
      this.output.send([0xF0, 0x7E, 0x7F, 0x06, 0x01, 0xF7])
    }
    else {
      console.log('No output or input')
    }
  }

}
