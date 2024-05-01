import { Routes } from '@angular/router';
import { DevicesComponent } from './devices/devices.component';

export const routes: Routes = [

  { path: 'devices', component: DevicesComponent },
  { path: '**', redirectTo: 'devices'}
];
