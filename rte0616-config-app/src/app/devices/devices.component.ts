import { Component, Inject, OnInit } from '@angular/core';
import { MIDI_INPUTS, MIDI_OUTPUTS } from '@ng-web-apis/midi';
import { Observable } from 'rxjs';
import { MatSelectModule } from '@angular/material/select';
import { AsyncPipe, NgFor, NgIf } from '@angular/common';
import { MatButtonModule } from '@angular/material/button';
import { MatProgressSpinner } from '@angular/material/progress-spinner'
import { MatSnackBar } from '@angular/material/snack-bar'
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
  fwVersion: string|undefined = undefined

  constructor(
    private _midiService: MidiService,
    private _snackBar: MatSnackBar,
    @Inject(MIDI_INPUTS) protected _inputs$: Observable<MIDIInput[]>,
    @Inject(MIDI_OUTPUTS) protected _outputs$: Observable<MIDIOutput[]>,
  )
  {}

  ngOnInit(): void {
    
  }

  selectDevice(ev: any, dev: MIDIOutput) {
    this.fwVersion = undefined
    this.selectedDevice = dev
    if (dev.name)
      this._midiService.selectDevice(dev.name)  
  }

  deviceSelected(): boolean {
    return this.selectedDevice != undefined;
  }

  identify() {
    this.identifying = true
    this._midiService.sendIdentSysex().then(value => {
      console.log('Firmware version: ' + value)
      this.fwVersion = value.toString();
      this._snackBar.open('Device identified !', undefined, {
        duration: 2000,
        panelClass: [ 'mat-primary']
      })
    })
    .catch(reason => this._snackBar.open(reason, undefined, {
      duration: 2000,
      panelClass: [ 'error-snackbar' ]
    }))
    .finally(() => this.identifying = false)
  }
}
