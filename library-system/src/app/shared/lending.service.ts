import { Injectable } from '@angular/core';
import { FormGroup, FormControl, Validators } from '@angular/forms';
import { Observable } from 'rxjs';
import { Lending } from '../lendings/lending.model';
import { HttpClient, HttpParams } from '@angular/common/http';
import { LoginService } from './login.service';

@Injectable({
  providedIn: 'root'
})
export class LendingService {

  constructor(private http: HttpClient) { 
  }

  lendingList=[];
  isEditAction: boolean;
  finePerDay: number;
  issuePeriod: number;

  lend_form = new FormGroup({
    userId: new FormControl('', [Validators.required, Validators.pattern("^[0-9a-zA-Z ]*$")]),
    bookCode: new FormControl('', [Validators.required, Validators.pattern("^[0-9a-zA-Z ]*$")])
  });

  settings_form = new FormGroup({
    finePerDay: new FormControl('', Validators.required),
    issuePeriod: new FormControl('', Validators.required)
  });

  getLendings(): Observable<Lending[]> {
    const params = new HttpParams().set('libraryId', LoginService.libraryId);
    return this.http.get<Lending[]>('http://localhost:9002/api/lendings', { params });   
  }

  addLending(lending) {
    if(this.isEditAction){
      const params = new HttpParams().set('libraryId', LoginService.libraryId).set('userId', lending.userId)
      .set('bookCode', lending.bookCode).set('lendId', lending.lendId).set('isReturned', '0');
        return this.http.put<Lending[]>('http://localhost:9002/api/lendings', lending, { params });
    }
    else{
      console.log("add fine:" + this.finePerDay + "  period:" + this.issuePeriod);
      const params = new HttpParams().set('libraryId', LoginService.libraryId).set('userId', lending.userId)
      .set('bookCode', lending.bookCode).set('issuePeriod', String(this.issuePeriod));
        return this.http.post<Lending[]>('http://localhost:9002/api/lendings', lending, { params });
    }
  }

  populateUserForm(lending){
    this.isEditAction = true;
    this.lend_form.setValue(lending);
  }

  returnBook(lending) {
      console.log("Return book with code " + lending.bookCode);
      const params = new HttpParams().set('libraryId', LoginService.libraryId).set('userId', lending.userId)
      .set('bookCode', lending.bookCode).set('lendId', lending.lendId).set('isReturned', '1');
      return this.http.put<Lending[]>('http://localhost:9002/api/lendings', lending, { params });
  }

  updateSettings(settings){
    console.log("update setting");
    console.log("fine:" + settings.finePerDay + "  period:" + settings.issuePeriod);
    const params = new HttpParams().set('libraryId', LoginService.libraryId).set('libraryId', 'testlibrary')
      .set('finePerDay', settings.finePerDay).set('issuePeriod', settings.issuePeriod);
      return this.http.put<Lending[]>('http://localhost:9003/api/configs', settings, { params });
  }
}
