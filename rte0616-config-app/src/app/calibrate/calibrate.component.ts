import { Component, OnInit } from '@angular/core';
import { TabsComponent } from '../tabs/tabs.component';
import { DeviceCalibration, MidiService } from '../midi.service';

@Component({
  selector: 'app-calibrate',
  standalone: true,
  imports: [ TabsComponent ],
  templateUrl: './calibrate.component.html',
  styleUrl: './calibrate.component.css'
})
export class CalibrateComponent implements OnInit {
  protected calibration = new DeviceCalibration()

  constructor(
    private _midiService: MidiService)
  {}

    ngOnInit(): void {
      this._midiService.calibration$.subscribe(val => { this.calibration = val })
      this._midiService.sendMeasureModeRequest()
      this._midiService.sendGetCalibrationRequest()
  }
}
