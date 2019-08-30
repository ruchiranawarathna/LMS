import { Component, OnInit } from '@angular/core';
import { UserService } from '../shared/user.service';

@Component({
  selector: 'app-users',
  templateUrl: './users.component.html',
  styleUrls: ['./users.component.css']
})
export class UsersComponent implements OnInit {

  constructor(private userService: UserService) { }

  ngOnInit() {
  }

  submitted: boolean;
  showSuccessMessage: boolean;
  showFailureMessage: boolean;
  duplicateUserID: boolean;
  statusCode: number;
  formControls = this.userService.user_form.controls;

  onSubmit() {
    this.submitted = true;
    if(this.userService.user_form.valid)
    {      
        this.userService.addUser(this.userService.user_form.value)
        .subscribe(
          // (response) => console.log(response),
          (data) => this.onSuccess(data),
          (error) => this.handleError(error)
        );     
      this.submitted = false;
      this.userService.user_form.reset();
      this.userService.isEditAction = false;      
    }
  }

  onSuccess(data: any)
  {
    this.userService.userList = data;
    this.showSuccessMessage = true;
    setTimeout(() => this.showSuccessMessage = false, 3000);
  }

  handleError(error: Response){
    this.statusCode = error.status;
    if(this.statusCode == 800){
      this.duplicateUserID = true;
      console.log("User ID already exist");
      setTimeout(() => this.duplicateUserID = false, 3000);
    }
  }
}
