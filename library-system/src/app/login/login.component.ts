import { Component, OnInit } from '@angular/core';
import { LoginService } from '../shared/login.service';

@Component({
  selector: 'app-login',
  templateUrl: './login.component.html',
  styleUrls: ['./login.component.css']
})
export class LoginComponent implements OnInit {

  constructor(private loginService: LoginService) { }

  ngOnInit() {
  }

  // submitted: boolean;
  // showSuccessMessage: boolean;
  // showFailureMessage: boolean;
  // statusCode: number;
  formControls = this.loginService.login_form.controls;

  onSubmit() {
    console.log("--------------- login onSubmit");
      this.loginService.login(this.loginService.login_form.value); 
  }
}

