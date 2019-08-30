import { Component, OnInit } from '@angular/core';
import { BookService } from '../shared/book.service';
import { ngxCsv } from 'ngx-csv/ngx-csv';

import { Book } from '../books/book.model'

@Component({
  selector: 'app-books-list',
  templateUrl: './books-list.component.html',
  styleUrls: ['./books-list.component.css']
})
export class BooksListComponent implements OnInit {

  constructor(private bookService: BookService) { }
 showClicked: boolean;
 downloadCompleted: boolean;

  ngOnInit() {
    
  }

  onGetBooks(){
    this.showClicked = true;
    this.bookService.getBooks().subscribe(
      data => this.bookService.bookList = data);
  }

  onDelete(book: Book){
    console.log("Delete book with code " + book.code);
    if(confirm('Are you sure to delete this record?')){
      this.bookService.removeBook(book).subscribe(
        data => this.bookService.bookList = data,
        // (response) => console.log(response),
        (error) => console.log(error)
      );
    }
    // this.showClicked = false;
  }

  onEdit(book: Book){
    console.log("Edit book with code " + book.code);
    this.bookService.populateBookForm(book);
    // this.showClicked = false;
  }

  onDownload(){
    console.log("Download books");
    this.bookService.getBooks().subscribe(data => this.downloadFile(data)),
    error => console.log('Error downloading the file.'),
    () => console.info('OK');

    this.downloadCompleted = true;
    setTimeout(() => this.downloadCompleted = false, 3000);
  }

  downloadFile(data: any) {
    var options = { 
      headers: ["Author", "Category", "Code", "Title"]
    };
    new ngxCsv(data, 'Book_List', options);
  }

 
}
