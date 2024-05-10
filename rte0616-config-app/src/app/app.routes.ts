import { Routes } from '@angular/router';
import { DevicesComponent } from './devices/devices.component';
import { ConfigureComponent } from './configure/configure.component';
import { CalibrateComponent } from './calibrate/calibrate.component';
import { TabsComponent } from './tabs/tabs.component';
import { DeviceSelectedGuard } from './midi.service';

export const routes: Routes = [
  {
    path: '',
    component: TabsComponent,
    canActivate: [DeviceSelectedGuard],
    children: [
      { path: 'configure', component: ConfigureComponent },
      { path: 'calibrate', component: CalibrateComponent },
    ]
  },
  { path: 'devices', component: DevicesComponent },
  { path: '**', redirectTo: 'devices'}
]