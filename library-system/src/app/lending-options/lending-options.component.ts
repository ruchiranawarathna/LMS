import { Component, OnInit } from '@angular/core';
import { LendingService } from '../shared/lending.service';

@Component({
  selector: 'app-lending-options',
  templateUrl: './lending-options.component.html',
  styleUrls: ['./lending-options.component.css']
})
export class LendingOptionsComponent implements OnInit {

  constructor(private lendingService: LendingService) { }

  ngOnInit() {
  }

  formControls = this.lendingService.settings_form.controls;
  showSuccessMessage: boolean;
  showSettings: boolean;

  onUpdateSettings()
  {
    this.showSettings = true;
  }

  onOptionChange()
  {
    this.lendingService.updateSettings(this.lendingService.settings_form.value).subscribe(
      (data) => this.onSuccess(data)),
      (error) => this.handleError(error);
    this.showSettings = false;
  }

  onSuccess(data: any)
  {
    this.lendingService.lendingList = data;
    this.showSuccessMessage = true;
    setTimeout(() => this.showSuccessMessage = false, 3000);
  }

  handleError(error: Response){
    console.log(error);
  }

  
}
