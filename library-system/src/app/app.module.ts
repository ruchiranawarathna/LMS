import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { MatToolbarModule } from '@angular/material/toolbar';
import { MatTabsModule } from '@angular/material/tabs';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';

import { AppComponent } from './app.component';
import { HeaderComponent } from './header/header.component';
import { BooksComponent } from './books/books.component';
import { UsersComponent } from './users/users.component';
import { BooksListComponent } from './books-list/books-list.component';
import { ReactiveFormsModule } from '@angular/forms';
import { BookService } from './shared/book.service';
import { HttpModule } from '@angular/http';
import { HttpClientModule } from '@angular/common/http';
import { UsersListComponent } from './users-list/users-list.component';
import { LendingsComponent } from './lendings/lendings.component';
import { LendingsListComponent } from './lendings-list/lendings-list.component'
import { FormsModule } from '@angular/forms';
import { LendingOptionsComponent } from './lending-options/lending-options.component';
import { LoginComponent } from './login/login.component';

@NgModule({
  declarations: [
    AppComponent,
    HeaderComponent,
    BooksComponent,
    UsersComponent,
    BooksListComponent,
    UsersListComponent,
    LendingsComponent,
    LendingsListComponent,
    LendingOptionsComponent,
    LoginComponent
  ],
  imports: [
    BrowserModule,
    MatToolbarModule,
    MatTabsModule,
    BrowserAnimationsModule,
    ReactiveFormsModule,
    HttpModule,
    HttpClientModule,
    FormsModule
    
  ],
  providers: [BookService],
  bootstrap: [AppComponent]
})
export class AppModule { }
