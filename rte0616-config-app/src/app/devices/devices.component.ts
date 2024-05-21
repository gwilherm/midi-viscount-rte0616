import { Component, Inject, OnInit } from '@angular/core';
import { MIDI_INPUTS, MIDI_OUTPUTS } from '@ng-web-apis/midi';
import { Observable } from 'rxjs';
import { MatSelectModule } from '@angular/material/select';
import { AsyncPipe, NgFor, NgIf } from '@angular/common';
import { MatButtonModule } from '@angular/material/button';
import { MatProgressSpinner } from '@angular/material/progress-spinner'
import { MatSnackBar } from '@angular/material/snack-bar'
import { FirmwareVersion, MidiService } from '../midi.service';
import { Router } from '@angular/router';

@Component({
  selector: 'app-devices',
  standalone: true,
  imports: [NgIf, NgFor, AsyncPipe, MatSelectModule, MatButtonModule, MatProgressSpinner],
  templateUrl: './devices.component.html',
  styleUrl: './devices.component.css',
  })

export class DevicesComponent implements OnInit {
  selectedDevice: MIDIOutput | undefined;
  identifying = false;
  fwVersion: string|undefined = undefined

  constructor(
    protected _router: Router,
    private _snackBar: MatSnackBar,
    private _midiService: MidiService,
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
    this._midiService.sendIdentRequest().then(fwVersion => {
      console.log('Firmware version: ' + fwVersion)
      this.fwVersion = fwVersion.toString();
      this._snackBar.open('Viscount RTE0616 identified !', undefined, {
        duration: 2000,
        panelClass: [ 'mat-primary']
      })
    })
    .catch(reason => this._snackBar.open(reason, undefined, {
      duration: 2000,
      panelClass: [ /* 'mat-warn' */ 'error-snackbar' ]
    }))
    .finally(() => this.identifying = false)
  }

  onContinue() {
    this._router.navigateByUrl('/configure')
  }
}
