export class Lending {
    public lendId: string;
    public userId: string;
    public bookCode: string;
    public issueDate: Date;
    public dueDate: Date;
    public delay: number;
    public fine: number;
    public isReturned: boolean;

    constructor(id: string, userId: string, bookCode: string, issueDate: Date) {
        this.lendId = id;
        this.userId = userId;
        this.bookCode = bookCode;
        // this.issueDate = issueDate;
    }
} 