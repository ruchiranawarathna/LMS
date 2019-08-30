import { Component, OnInit } from '@angular/core';
import { UserService } from '../shared/user.service';
import { User } from '../users/user.model';
import { ngxCsv } from 'ngx-csv/ngx-csv';

@Component({
  selector: 'app-users-list',
  templateUrl: './users-list.component.html',
  styleUrls: ['./users-list.component.css']
})
export class UsersListComponent implements OnInit {

  constructor(private userService: UserService) { }

  ngOnInit() {
  }

  showClicked: boolean;
  downloadCompleted: boolean;

  onGetUsers(){
    this.showClicked = true;
    this.userService.getUsers().subscribe(
      data => this.userService.userList = data);
  }

  onDelete(user: User){
    console.log("Delete user with id " + user.userId);
    if(confirm('Are you sure to delete this record?')){
      this.userService.removeUser(user).subscribe(
        // (response) => console.log(response),
        data => this.userService.userList = data,
        (error) => console.log(error)
      );
    }
  }

  onEdit(user: User){
    console.log("Edit user with id " + user.userId);
    this.userService.populateUserForm(user);
  }

  onDownload(){
    console.log("Download users");
    this.userService.getUsers().subscribe(data => this.downloadFile(data)),
    error => console.log('Error downloading the file.'),
    () => console.info('OK');
    this.downloadCompleted = true;
    setTimeout(() => this.downloadCompleted = false, 3000);
  }

  downloadFile(data: any) {
    var options = { 
      headers: ["Address", "Contact", "Name", "User ID"]
    };
    new ngxCsv(data, 'User_List', options);
  }
}
