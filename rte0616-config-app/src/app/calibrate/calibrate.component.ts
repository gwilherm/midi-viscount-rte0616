import { Component, OnInit } from '@angular/core';
import { TabsComponent } from '../tabs/tabs.component';
import { DeviceCalibration, DeviceMeasures as DeviceMeasures, MidiService } from '../midi.service';

@Component({
  selector: 'app-calibrate',
  standalone: true,
  imports: [ TabsComponent ],
  templateUrl: './calibrate.component.html',
  styleUrl: './calibrate.component.css'
})
export class CalibrateComponent implements OnInit {
  protected calibration = new DeviceCalibration()
  protected measures = new DeviceMeasures()

  constructor(
    private _midiService: MidiService)
  {}

    ngOnInit(): void {
      this._midiService.calibration$.subscribe(val => { this.calibration = val })
      this._midiService.measure$.subscribe(val => { this.measures = val })
      this._midiService.sendMeasureModeRequest()
      this._midiService.sendGetCalibrationRequest()
  }
}
