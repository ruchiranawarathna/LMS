import { Injectable } from '@angular/core';
import { HttpClient, HttpParams } from '@angular/common/http';
import { FormGroup, Validators, FormControl } from '@angular/forms';
import { User } from '../users/user.model';
import { Observable } from 'rxjs';
import { LoginService } from './login.service';

@Injectable({
  providedIn: 'root'
})
export class UserService {

  userList=[];
  isEditAction: boolean;

  constructor(private http: HttpClient) { }

  user_form = new FormGroup({
    userId: new FormControl('', [Validators.required, Validators.pattern("^[0-9a-zA-Z ]*$")]),
    name: new FormControl('', [Validators.required, Validators.pattern("^[a-zA-Z ]*$")]),
    contact: new FormControl('0123456789', [Validators.required, Validators.minLength(10), Validators.maxLength(10), Validators.pattern("^[0-9]*$")]),
    address: new FormControl('', [Validators.required, Validators.pattern("^[0-9a-zA-Z,/ ]*$")])
  });


  getUsers(): Observable<User[]> {
    const params = new HttpParams().set('libraryId', LoginService.libraryId);
    return this.http.get<User[]>('http://localhost:9001/api/users', { params });   
  }

  addUser(user) {
    const params = new HttpParams().set('libraryId', LoginService.libraryId).set('userId', user.userId)
      .set('name', user.name).set('contact', user.contact).set('address', user.address);

    if(this.isEditAction)
      return this.http.put<User[]>('http://localhost:9001/api/users', user, { params });
    else
      return this.http.post<User[]>('http://localhost:9001/api/users', user, { params });

  }

  populateUserForm(user){
    this.isEditAction = true;
    this.user_form.setValue(user);
  }

  removeUser(user: User){
    const params = new HttpParams().set('libraryId', LoginService.libraryId).set('userId', user.userId)
      .set('name', user.name).set('contact', user.contact).set('address', user.address);
    return this.http.delete<User[]>('http://localhost:9001/api/users' , { params });
  }
}
