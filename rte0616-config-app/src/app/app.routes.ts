import { Routes } from '@angular/router';
import { DevicesComponent } from './devices/devices.component';
import { ConfigureComponent } from './configure/configure.component';

export const routes: Routes = [

  { path: 'devices', component: DevicesComponent },
  { path: 'configure', component: ConfigureComponent },
  { path: '**', redirectTo: 'devices'}
];
