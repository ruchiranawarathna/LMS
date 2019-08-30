export class Book {
    public code: string;
    public title: string;
    public author: string;
    public category: string; 

    constructor(code: string, title: string, auth: string, cat: string) {
        this.code = code;
        this.title = title;
        this.author = auth;
        this.category = cat;
    }
} 