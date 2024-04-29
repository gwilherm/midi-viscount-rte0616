import { ApplicationConfig } from '@angular/core';
import { provideRouter } from '@angular/router';

import { routes } from './app.routes';
import { SYSEX } from '@ng-web-apis/midi';

export const appConfig: ApplicationConfig = {
  providers: [
    provideRouter(routes),
    {
      provide: SYSEX,
      useValue: true
    }
  ]
};
