import { Component, Inject, OnInit, inject } from '@angular/core';
import { RouterOutlet } from '@angular/router';
import { DevicesComponent } from './devices/devices.component';

@Component({
  selector: 'app-root',
  standalone: true,
  imports: [RouterOutlet, DevicesComponent],
  templateUrl: './app.component.html',
  styleUrl: './app.component.css',
})

export class AppComponent implements OnInit {
  title = 'rte0616-config-app';

  constructor()
  {

  }
  
  ngOnInit(): void {
    // console.log('WebMIDI: ' + this.supported)
  }
}
