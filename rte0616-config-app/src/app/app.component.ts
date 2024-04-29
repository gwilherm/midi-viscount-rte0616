import { Component, Inject, OnInit, inject } from '@angular/core';
import { RouterOutlet } from '@angular/router';
import { MIDI_INPUTS, MIDI_OUTPUTS } from '@ng-web-apis/midi';
import { Observable } from 'rxjs';

@Component({
  selector: 'app-root',
  standalone: true,
  imports: [RouterOutlet],
  templateUrl: './app.component.html',
  styleUrl: './app.component.css',
})

export class AppComponent implements OnInit {
  title = 'rte0616-config-app';

  constructor(
    @Inject(MIDI_INPUTS) inputs: Observable<MIDIInput[]>,
    @Inject(MIDI_OUTPUTS) outputs: Observable<MIDIOutput[]>,
  )
  {
    inputs.subscribe({
      next: inputs => { inputs.filter(i => i.name?.startsWith('Viscount RTE0616'))[0] }
    })

    outputs.subscribe({
      next: outputs => { 
        console.log(outputs)
        const output = outputs.filter(o => o.name?.startsWith('Viscount RTE0616'))[0]
        if (output) {
          console.log(output.name)
          output.send([0xF0, 0x7E, 0x7F, 0x06, 0x01, 0xF7])
        }
      }
    })
  }
  
  ngOnInit(): void {
    // console.log('WebMIDI: ' + this.supported)
  }
}
