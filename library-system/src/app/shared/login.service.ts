import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { FormGroup, FormControl, Validators } from '@angular/forms';

@Injectable({
  providedIn: 'root'
})
export class LoginService {

  isLoginSuccess: boolean;
  static libraryId = "";
  
  constructor(private http: HttpClient) { }

  login_form = new FormGroup({
    libraryId: new FormControl('', [Validators.required, Validators.pattern("^[0-9a-zA-Z ]*$")])
    // password: new FormControl('', [Validators.required, Validators.pattern("^[0-9a-zA-Z ]*$")])
  });

  login(login) {
    console.log("---------------- loginService login:" + login.libraryId);
    this.isLoginSuccess = true;
    LoginService.libraryId = login.libraryId;
  }
}
