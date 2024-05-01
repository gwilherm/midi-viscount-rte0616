import { Component, Inject, OnInit } from '@angular/core';
import { MIDI_INPUTS, MIDI_OUTPUTS } from '@ng-web-apis/midi';
import { Observable } from 'rxjs';
import { MatSelectModule } from '@angular/material/select';
import { AsyncPipe, NgFor, NgIf } from '@angular/common';
import { MatButtonModule } from '@angular/material/button';
import { MatProgressSpinner } from '@angular/material/progress-spinner'

@Component({
  selector: 'app-devices',
  standalone: true,
  imports: [NgIf, NgFor, AsyncPipe, MatSelectModule, MatButtonModule,MatProgressSpinner],
  templateUrl: './devices.component.html',
  styleUrl: './devices.component.css'
})
export class DevicesComponent implements OnInit {
  inputs$: Observable<MIDIInput[]>;
  outputs$: Observable<MIDIOutput[]>;
  selectedDevice: MIDIOutput | undefined;
  identifying = false;

  constructor(
    @Inject(MIDI_INPUTS) inputs$: Observable<MIDIInput[]>,
    @Inject(MIDI_OUTPUTS) outputs$: Observable<MIDIOutput[]>,
  )
  {
    this.inputs$ = inputs$
    this.outputs$ = outputs$

    // outputs.subscribe({
    //   next: outputs => { 
    //     console.log(outputs)
    //     const output = outputs.filter(o => o.name?.startsWith('Viscount RTE0616'))[0]
    //     if (output) {
    //       console.log(output.name)
    //       output.send([0xF0, 0x7E, 0x7F, 0x06, 0x01, 0xF7])
    //     }
    //   }
    // })
  }

  ngOnInit(): void {

  }

  selectDevice(ev: any, dev: MIDIOutput) {
    this.selectedDevice = dev
  }

  deviceSelected(): boolean {
    return this.selectedDevice != undefined;
  }

  identify() {
    this.identifying = true
    setTimeout(() => this.identifying = false, 2000)
  }
}
