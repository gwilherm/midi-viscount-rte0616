import { AfterViewInit, Component, QueryList, ViewChildren } from '@angular/core';
import { DeviceCalibration, DeviceMeasures as DeviceMeasures, MidiService } from '../midi.service';
import { IgxLinearGaugeComponent, IgxLinearGaugeModule, IgxLinearGraphRangeComponent, LinearGraphNeedleShape } from 'igniteui-angular-gauges';
import { NgFor } from '@angular/common';
import { MatInputModule } from '@angular/material/input';
import { FormsModule } from '@angular/forms';
import { MatButtonModule } from '@angular/material/button';

@Component({
  selector: 'app-calibrate',
  standalone: true,
  imports: [ MatInputModule, MatButtonModule, FormsModule, IgxLinearGaugeModule, NgFor ],
  templateUrl: './calibrate.component.html',
  styleUrl: './calibrate.component.css'
})
export class CalibrateComponent implements AfterViewInit {
  @ViewChildren("gauge") public gaugeList!: QueryList<IgxLinearGaugeComponent>
  NB_GAUGE = 8
  gaugeIdx = Array(this.NB_GAUGE).fill(1).map((x,i)=>i)


  protected _calibration = new DeviceCalibration()
  protected _measures = new DeviceMeasures()

  constructor(
    private _midiService: MidiService)
    {}

    initializeGauge(gauge: IgxLinearGaugeComponent) {
      gauge.transitionDuration = 100

      gauge.rangeBrushes  = [ "#C39B74", "#9CC374", "#749CC3", "#9B74C3" ]
      gauge.rangeOutlines = [ "#C39B74", "#9CC374", "#749CC3", "#9B74C3" ]

      gauge.minimumValue = 0
      gauge.maximumValue = 1024
      gauge.value = 1024
      gauge.interval = 128

      // setting custom appearance of labels
      gauge.labelInterval = 256
      gauge.labelExtent = 0.05

      // setting custom appearance of needle
      gauge.needleShape = LinearGraphNeedleShape.Needle
      gauge.needleBrush = "#79797a"
      gauge.needleOutline = "#ffffffff"
      gauge.needleStrokeThickness = 1
      gauge.needleOuterExtent = 0.9
      gauge.needleInnerExtent = 0.3

      // setting custom appearance of major/minor ticks
      gauge.minorTickCount = 5
      gauge.minorTickEndExtent = 0.10
      gauge.minorTickStartExtent = 0.20
      gauge.minorTickStrokeThickness = 1
      gauge.tickStartExtent = 0.25
      gauge.tickEndExtent = 0.05
      gauge.tickStrokeThickness = 2

      // setting extent of gauge scale
      gauge.scaleStrokeThickness = 0
      gauge.scaleBrush = "#ffffff"
      gauge.scaleOutline = "#dbdbdb"
      gauge.scaleInnerExtent = 0.075
      gauge.scaleOuterExtent = 0.85
      gauge.scaleStartExtent = 0.05
      gauge.scaleEndExtent = 0.95

      // setting appearance of backing fill and outline
      gauge.backingBrush = "#ffffff"
      gauge.backingOutline = "#d1d1d1"
      gauge.backingStrokeThickness = 0
    }
    
    ngAfterViewInit(): void {
      for (let i = 0; i < this.NB_GAUGE; i++) {
        this.initializeGauge(this.gaugeList.get(i)!)
      }

      this._midiService.calibration$.subscribe(cal => {
        this._calibration = cal

        const segRange = new Array<IgxLinearGraphRangeComponent>()
        cal.vSeg.forEach(vSeg => {
          const seg = new IgxLinearGraphRangeComponent()
          seg.startValue = vSeg - cal.margin
          seg.endValue = vSeg + cal.margin

          seg.innerStartExtent = 0.075
          seg.innerEndExtent = 0.075
          seg.outerStartExtent = 0.65
          seg.outerEndExtent = 0.65

          segRange.push(seg)
        })
         
        // setting extent of all gauge ranges
        for (let i = 0; i < this.NB_GAUGE; i++) {
          this.gaugeList.get(i)!.ranges.clear()
          segRange.forEach(range => this.gaugeList.get(i)!.ranges.add(range))
        }
      })

      this._midiService.measures$.subscribe(meas => {
        this._measures = meas
        for (let i = 0; i < this.NB_GAUGE; i++) {
          this.gaugeList.get(i)!.value = meas.v[i]
        }
      })

      this._midiService.sendMeasureModeRequest()
      this._midiService.sendGetCalibrationRequest()
  }

  onSend() {
    this._midiService.sendSetCalibrationRequest(this._calibration)
  }
}
