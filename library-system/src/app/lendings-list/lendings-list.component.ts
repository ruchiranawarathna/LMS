import { Component, OnInit } from '@angular/core';
import { LendingService } from '../shared/lending.service';
import { Lending } from '../lendings/lending.model';
import { ngxCsv } from 'ngx-csv/ngx-csv';

@Component({
  selector: 'app-lendings-list',
  templateUrl: './lendings-list.component.html',
  styleUrls: ['./lendings-list.component.css']
})
export class LendingsListComponent implements OnInit {

  constructor(private lendingService: LendingService) { }

  ngOnInit() {
  }

  showClicked: boolean;
  downloadCompleted: boolean;

  onGetLends(){
    this.showClicked = true;
    this.lendingService.getLendings().subscribe(
      data => this.lendingService.lendingList = data);
  }

  onEdit(lending: Lending){
    this.lendingService.isEditAction = true;
    this.lendingService.populateUserForm(lending);
  }

  onReturn(lending: Lending){
    this.lendingService.returnBook(lending).subscribe(
      data => this.lendingService.lendingList = data);
  }
  
  onDownload(){
    console.log("Download lendings");
    this.lendingService.getLendings().subscribe(data => this.downloadFile(data)),
    error => console.log('Error downloading the file.'),
    () => console.info('OK');
    this.downloadCompleted = true;
    setTimeout(() => this.downloadCompleted = false, 3000);
  }

  downloadFile(data: any) {
    var options = { 
      headers: ["Book Code", "Delay", "Due Date", "Fine (Rs.)", "Issue Date", "Lend ID", "User ID"]
    };
    new ngxCsv(data, 'Lending_List', options);
  }
  
}
