import { Component, OnInit } from '@angular/core';
import { Router, RouterOutlet } from '@angular/router';
import { DevicesComponent } from './devices/devices.component';
import { TabsComponent } from './tabs/tabs.component';
import { NgFor, NgIf } from '@angular/common';
import { MatTabsModule } from '@angular/material/tabs';

@Component({
  selector: 'app-root',
  standalone: true,
  imports: [RouterOutlet, MatTabsModule, DevicesComponent, TabsComponent, NgIf, NgFor ],
  templateUrl: './app.component.html',
  styleUrl: './app.component.css',
})
export class AppComponent implements OnInit {
  title = 'rte0616-config-app';

  constructor(protected _router: Router) {}
  
  ngOnInit(): void {}
}
