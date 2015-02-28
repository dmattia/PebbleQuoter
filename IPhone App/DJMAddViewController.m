//
//  DJMAddViewController.m
//  Quoter
//
//  Created by David Mattia on 2/28/15.
//  Copyright (c) 2015 David Mattia. All rights reserved.
//

#import "DJMAddViewController.h"
#import <Parse/Parse.h>

@interface DJMAddViewController ()

@property (weak, nonatomic) IBOutlet UITextField *textBox;

@end

@implementation DJMAddViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)test:(id)sender {
    if(self.textBox.text.length > 0 && self.textBox.text.length < 55) {
        NSLog(@"%@",self.textBox.text);
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Great Job!"
                                                        message:@"Your quote has been posted to our database."
                                                       delegate:nil
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles:nil];
        [alert show];
        
        //Add to the database
        PFObject *testObject = [PFObject objectWithClassName:@"QuoteObject"];
        testObject[@"quotes"] = self.textBox.text;
        [testObject saveInBackground];
        
    } else if (self.textBox.text.length > 55) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Oh No!"
                                                        message:@"Please make sure your quote is less than 55 characters"
                                                       delegate:nil
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles:nil];
        [alert show];
    } else {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Oh No!"
                                                        message:@"You need to type a quote before submitting"
                                                       delegate:nil
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles:nil];
        [alert show];
    }
}

@end
