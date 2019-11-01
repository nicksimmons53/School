package game;

import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;
import java.util.Random;

public class War_game {
	
	Card p1Card;
	Card p2Card;
	
	public War_game(LinkedList<Card> deck1, LinkedList<Card> deck2, Player player1, Player player2){
		
        while(true){
            Card p1Card = deck1.pop();  //each player place one card face up
            Card p2Card = deck2.pop();
            
            //display the face up card
            System.out.println( player1.getName() + " plays card is " + p1Card.toString());
            System.out.println(player2.getName() + " plays card is " + p2Card.toString());
            
            //rank comparation between two cards
            if(p1Card.getRank() > p2Card.getRank()){//if player 1 win 
                deck1.addLast(p1Card);  //higher rank wins both cards and 
                deck1.addLast(p2Card);  //places them at the bottom of his deck.
                System.out.println(player1.getName() + " wins the round");
            }//end if
 
            else if(p1Card.getRank() < p2Card.getRank()){//if player 2 win 
                deck2.addLast(p1Card);   
                deck2.addLast(p2Card);  
                System.out.println(player2.getName() + " wins the round");
            }//end else if
            
            else {
            	List<Card> war1 = new ArrayList<Card>(); 
            	 
            	while(true){//war happens when both cards' rank matched
            		System.out.println("War"); 
                
       
                    //either one player runs out of card is game over
            		if(deck1.size() == 0 || deck2.size() == 0 ){ 
            			System.out.println("Cant continue War: Deck is out of cards");
            			break;
                  } else{
                	  System.out.println("War card for "+ player1.getName() + " is xx\n" + "War card for " + player2.getName() + " is xx");                     
                      war1.add(deck1.pop());  //place additional card for war
                      war1.add(deck2.pop());
                      
                      if(deck1.size() > 0 && deck2.size() > 0){
                    	  p1Card = deck1.pop();
                    	  p2Card = deck2.pop();
                      } else{
                    	  break;
                      }
                  }
                    
                    System.out.println("War card for " + player1.getName() + " is " + p1Card.toString());
                    System.out.println("War card for " + player2.getName() + " is " + p2Card.toString());
                    
                    //if player 1 wins the war round
                    if(p1Card.getRank() > p2Card.getRank()){
                        deck1.add(p2Card); 
                        deck1.add(p1Card);
                        deck1.addAll(war1);
                        System.out.println(player1.getName() + " wins the war round");
                        break;
                    }//end if
                    //otherwise player 2 wins the war round
                    else if(p2Card.getRank() > p1Card.getRank()){
                    	deck2.add(p2Card); 
                        deck2.add(p1Card);
                        deck2.addAll(war1);
                        System.out.println(player2.getName() + " wins the war round");
                        break;
                        //nobody won, continue to war again
                    } else{
                    	System.out.println("Tie: Continue War");//end else                      
                    }
            	}
                
            }//end war round else
            
            //game over either one player runs out of card(deck size is 0)
            if(deck1.size() == 0 ){
                System.out.println("game over\n" + player2.getName() +" wins the game");
                break;
            }
            else if(deck2.size() == 0){
                System.out.println("game over\n" + player1.getName() + " wins the game");
                break;
            }
        }//end while  

	}
}
