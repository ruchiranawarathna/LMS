import { Injectable } from '@angular/core';
import { FormControl, FormGroup, Validators } from '@angular/forms'
import { Headers, URLSearchParams, RequestOptions } from '@angular/http';
import { Http, Response } from '@angular/http';

import 'rxjs/Rx';

import { Book } from '../books/book.model'
import { HttpClient, HttpHeaders, HttpParams } from '@angular/common/http';
import { Observable } from 'rxjs/Observable';
import { LoginService } from './login.service';
@Injectable({
  providedIn: 'root'
})
export class BookService {

  bookList=[];

  isEditAction: boolean;

  constructor(private http: HttpClient) { }
  book_form = new FormGroup({
    code: new FormControl('', [Validators.required, Validators.pattern("^[0-9a-zA-Z ]*$")]),
    title: new FormControl('', [Validators.required, Validators.pattern("^[a-zA-Z ]*$")]),
    author: new FormControl('', [Validators.required, Validators.pattern("^[a-zA-Z ]*$")]),
    category: new FormControl('', Validators.required)
  });

  getBooks(): Observable<Book[]> {
    const params = new HttpParams().set('libraryId', LoginService.libraryId);
    return this.http.get<Book[]>('http://localhost:9000/api/books', { params });   
  }
  
  addBook(book) { 
    console.log("----------- lIbrary id:" + LoginService.libraryId);   
    const params = new HttpParams().set('libraryId', LoginService.libraryId).set('code', book.code).set('title', book.title).set('author', book.author).set('category', book.category);

    if(this.isEditAction)
      return this.http.put<Book[]>('http://localhost:9000/api/books', book, { params });
    else
      return this.http.post<Book[]>('http://localhost:9000/api/books', book, { params });
  }

  populateBookForm(book){
    this.isEditAction = true;
    this.book_form.setValue(book);
  }

  removeBook(book: Book){
    const params = new HttpParams().set('libraryId', LoginService.libraryId).set('code', book.code).set('title', book.title).set('author', book.author).set('category', book.category);
    return this.http.delete<Book[]>('http://localhost:9000/api/books' , { params });
  }
}
