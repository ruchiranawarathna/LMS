export class User {
    public userId: string;
    public name: string;
    public contact: string;
    public address: string; 

    constructor(userId: string, name: string, contact: string, address: string) {
        this.userId = userId;
        this.name = name;
        this.contact = contact;
        this.address = address;
    }
} 