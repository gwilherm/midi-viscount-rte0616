import { Component, OnInit } from '@angular/core';
import { NgFor } from '@angular/common';
import { MatInputModule } from '@angular/material/input';
import { MatButtonModule } from '@angular/material/button';
import { MatSelectModule } from '@angular/material/select';
import { MidiService, DeviceMIDIConfig, PDLBRD_MODE} from '../midi.service';
import { FormsModule } from '@angular/forms';

@Component({
  selector: 'app-configure',
  standalone: true,
  imports: [ MatInputModule, MatButtonModule, MatSelectModule, FormsModule, NgFor ],
  templateUrl: './configure.component.html',
  styleUrl: './configure.component.css',
})
export class ConfigureComponent implements OnInit {
  _midiConfig = new DeviceMIDIConfig()
  constructor(private _midiService: MidiService) {}

  _modes = [
      {value: PDLBRD_MODE.MODE_MONODIC_UP, viewValue: 'Monodic Up'},
      {value: PDLBRD_MODE.MODE_POLYPHONIC, viewValue: 'Polyphonic'}
  ]

  ngOnInit(): void {
    this._midiService.midiConfig$.subscribe(val => { 
      this._midiConfig = val
    })
    this._midiService.sendStandardModeRequest()
    this._midiService.sendGetConfigurationRequest()
  }

  onSend() {
    this._midiService.sendSetConfigurationRequest(this._midiConfig)
  }

  onStore() {
    this._midiService.sendStoreConfigurationRequest()
  }

  onChangeKeyboardMode() {

  }
}
