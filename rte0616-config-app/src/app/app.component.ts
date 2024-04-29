import { Component, Inject, OnInit, inject } from '@angular/core';
import { RouterOutlet } from '@angular/router';
import { MIDI_ACCESS, MIDI_INPUT, MIDI_OUTPUT, MIDI_SUPPORT, SYSEX, inputById, inputByName, outputByName } from '@ng-web-apis/midi';
import { MIDI_INPUTS, MIDI_OUTPUTS } from '@ng-web-apis/midi';
import { Observable } from 'rxjs';

@Component({
  selector: 'app-root',
  standalone: true,
  imports: [RouterOutlet],
  templateUrl: './app.component.html',
  styleUrl: './app.component.css',
  providers: [ { provide: SYSEX, useValue: true }]
})

export class AppComponent implements OnInit {
  title = 'rte0616-config-app';

  constructor(
    @Inject(SYSEX) sysex: boolean,
    @Inject(MIDI_ACCESS) access: Promise<MIDIAccess>,
    // @Inject(MIDI_INPUTS) inputs: Observable<MIDIInput[]>,
    @Inject(MIDI_OUTPUTS) outputs: Observable<MIDIOutput[]>,
  )
  {
    console.log(sysex)
    access.then(a => {
      console.log(a)
    })
    // inputs.subscribe({
    //   next: inputs => { inputs.filter(i => i.name?.startsWith('Viscount RTE0616'))[0] }
    // })
    outputs.subscribe({
      next: outputs => { 
        console.log(outputs)
        const output = outputs.filter(o => o.name?.startsWith('receivemidi'))[0]
        console.log(output.name)
        // output.send([0x90, 60, 0x7f])
        output.send([0xF0, 0x7E, 0x7F, 0x06, 0x01, 0xF7])
      }
    })
  }
  
  ngOnInit(): void {
    // console.log('WebMIDI: ' + this.supported)
  }
}
