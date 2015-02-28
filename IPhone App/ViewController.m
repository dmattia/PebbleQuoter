//
//  ViewController.m
//  Quoter
//
//  Created by David Mattia on 2/28/15.
//  Copyright (c) 2015 David Mattia. All rights reserved.
//

#import "ViewController.h"
#import <Parse/Parse.h>

@interface ViewController ()

@property (weak, nonatomic) IBOutlet UILabel *displayedQuote;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // Find a quote to display
    PFQuery* quoteQuery = [PFQuery queryWithClassName:@"QuoteObject"];
    PFObject* parseQuote = [quoteQuery getFirstObject];
    _quote = parseQuote[@"quotes"];
    if(!_quote) {
        _quote = @"There are no more quotes to display :(";
    }

    
    // Set displayed quote to quote from parse
    _displayedQuote.text = _quote;
    
    // Remove displayed quote from database
    if(parseQuote) [parseQuote delete];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
