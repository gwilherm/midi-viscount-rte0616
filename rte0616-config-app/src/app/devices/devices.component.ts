import { Component, Inject, OnInit } from '@angular/core';
import { MIDI_INPUTS, MIDI_OUTPUTS } from '@ng-web-apis/midi';
import { Observable } from 'rxjs';
import { MatSelectModule } from '@angular/material/select';
import { AsyncPipe, NgFor, NgIf } from '@angular/common';
import { MatButtonModule } from '@angular/material/button';
import { MatProgressSpinner } from '@angular/material/progress-spinner'
import { MidiService } from '../midi.service';

@Component({
  selector: 'app-devices',
  standalone: true,
  imports: [NgIf, NgFor, AsyncPipe, MatSelectModule, MatButtonModule,MatProgressSpinner],
  templateUrl: './devices.component.html',
  styleUrl: './devices.component.css',
})

export class DevicesComponent implements OnInit {
  selectedDevice: MIDIOutput | undefined;
  identifying = false;

  constructor(
    private midiService: MidiService,
    @Inject(MIDI_INPUTS) protected inputs$: Observable<MIDIInput[]>,
    @Inject(MIDI_OUTPUTS) protected outputs$: Observable<MIDIOutput[]>,
  )
  {}

  ngOnInit(): void {

  }

  selectDevice(ev: any, dev: MIDIOutput) {
    this.selectedDevice = dev
    if (dev.name)
      this.midiService.selectDevice(dev.name)  
  }

  deviceSelected(): boolean {
    return this.selectedDevice != undefined;
  }

  identify() {
    this.identifying = true
    this.midiService.sendIdentSysex()
    setTimeout(() => this.identifying = false, 2000)
  }
}
