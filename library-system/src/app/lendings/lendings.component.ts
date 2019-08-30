import { Component, OnInit } from '@angular/core';
import { LendingService } from '../shared/lending.service';

@Component({
  selector: 'app-lendings',
  templateUrl: './lendings.component.html',
  styleUrls: ['./lendings.component.css']
})
export class LendingsComponent implements OnInit {

  constructor(private lendingService: LendingService) { }

  ngOnInit() {
    console.log('Lending component onInit');
    // this.lendingService.getJSON().subscribe(data => {
    //   console.log(data);
    // });
  }

  submitted: boolean;
  showSuccessMessage: boolean;
  showFailureMessage: boolean;
  invalidUserID: boolean;
  invalidBookCode: boolean;
  bookNotAvailable: boolean;
  statusCode: number;
  formControls = this.lendingService.lend_form.controls;

  onSubmit() {
    this.submitted = true;
    if(this.lendingService.lend_form.valid)
    {
      this.lendingService.addLending(this.lendingService.lend_form.value)
      .subscribe(
        (data) => this.onSuccess(data),
        // (response) => console.log(response),
        (error) => this.handleError(error)
      ); 
      
      this.submitted = false;
      this.lendingService.lend_form.reset();
      this.lendingService.isEditAction = false;  
    }
  }

  onSuccess(data: any)
  {
    this.lendingService.lendingList = data;
    this.showSuccessMessage = true;
    setTimeout(() => this.showSuccessMessage = false, 3000);
  }

  handleError(error: Response){
    this.statusCode = error.status;
    if(this.statusCode == 200){
      this.showSuccessMessage = true;
      setTimeout(() => this.showSuccessMessage = false, 3000);
    }
    else if(this.statusCode == 802){
      this.invalidUserID = true;
      console.log("Invalid User ID");
      setTimeout(() => this.invalidUserID = false, 3000);
    }
    else if(this.statusCode == 803){
      this.invalidBookCode = true;
      console.log("Invalid Book Code");
      setTimeout(() => this.invalidBookCode = false, 3000);
    }
    else if(this.statusCode == 804){
      this.bookNotAvailable = true;
      console.log("Book is not available");
      setTimeout(() => this.bookNotAvailable = false, 3000);
    }
  }
}
