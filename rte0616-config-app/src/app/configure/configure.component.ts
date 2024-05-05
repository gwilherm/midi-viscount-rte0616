import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { MatInputModule } from '@angular/material/input';
import { MidiService, DeviceMIDIConfig } from '../midi.service';

@Component({
  selector: 'app-configure',
  standalone: true,
  imports: [ MatInputModule ],
  templateUrl: './configure.component.html',
  styleUrl: './configure.component.css',
})
export class ConfigureComponent implements OnInit {
  _midiConfig = new DeviceMIDIConfig()

  constructor(
    private _router:Router,
    private _midiService: MidiService) {
      if (!_midiService.isDeviceSelected()) {
        console.log('redirecting')
        this._router.navigate(['devices']);
      }
  }

  ngOnInit(): void {
    this._midiService.midiConfig$.subscribe(val => this._midiConfig = val)
    this._midiService.sendStandardModeRequest()
    this._midiService.sendGetConfigurationRequest()
  }

  onSend() {
    this._midiService.sendSetConfigurationRequest(this._midiConfig)
  }
}
