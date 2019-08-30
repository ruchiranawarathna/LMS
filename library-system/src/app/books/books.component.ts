import { Component, OnInit } from '@angular/core';
import { BookService } from '../shared/book.service';
import { Response } from '@angular/http';

@Component({
  selector: 'app-books',
  templateUrl: './books.component.html',
  styleUrls: ['./books.component.css']
})
export class BooksComponent implements OnInit {

  constructor(private bookService: BookService) { }
  submitted: boolean;
  showSuccessMessage: boolean;
  showFailureMessage: boolean;
  duplicateBookCode: boolean;
  statusCode: number;
  formControls = this.bookService.book_form.controls;
  categories = ["novel", "short", "childern"];
  ngOnInit() {
  }

  onSubmit() {
    this.submitted = true;
    if(this.bookService.book_form.valid)
    {
      this.bookService.addBook(this.bookService.book_form.value)
      .subscribe(
        (data) => this.onSuccess(data),
        // (response) => console.log(response),
        (error) => this.handleError(error)
      );
      this.submitted = false;
      this.bookService.book_form.reset();
      this.bookService.isEditAction = false;      
    }
  }

  onSuccess(data: any)
  {
    this.bookService.bookList = data;
    this.showSuccessMessage = true;
    setTimeout(() => this.showSuccessMessage = false, 3000);
  }

  handleError(error: Response){
    this.statusCode = error.status;
    if(this.statusCode == 801){
      this.duplicateBookCode = true;
      console.log("Book code already exit");
      setTimeout(() => this.duplicateBookCode = false, 3000);
    }
  }
}
