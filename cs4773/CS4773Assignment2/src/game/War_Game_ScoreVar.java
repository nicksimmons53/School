package game;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

public class War_Game_ScoreVar {
	
	int countP1;
	int countP2; 
	
	public War_Game_ScoreVar(LinkedList<Card> deck1, LinkedList<Card> deck2, Player player1, Player player2){
		
		int countP1 = 0;
		int countP2 = 0;
		
		 while(true){
	            Card p1Card = deck1.pop();  //each player place one card face up
	            Card p2Card = deck2.pop();
	            
	            //display the face up card
	            System.out.println(player1.getName() + " plays card is " + p1Card.toString());
	            System.out.println(player2.getName() + " plays card is " + p2Card.toString());
	            
	            //rank comparation between two cards
	            if(p1Card.getRank() > p2Card.getRank()){//if player 1 win 
	                System.out.println(player1.getName() + " wins the round");
	                countP1 += 2;
	            }//end if
	 
	            else if(p1Card.getRank() < p2Card.getRank()){//if player 2 win 
	                System.out.println(player2.getName() + " wins the round");
	                countP2 += 2;
	            }//end else if
	            
	            else {
	            	List<Card> war1 = new ArrayList<Card>(); 
	            	 
	            	while(true){//war happens when both cards' rank matched
	            		System.out.println("War"); 
	            		war1.add(p1Card);
	            		war1.add(p2Card);
	                 
	                    //either one player runs out of card is game over
	            		if(deck1.size() == 0 || deck2.size() == 0 ){ 
	            			System.out.println("Can't continue War: Deck is out of cards");
	            			if(deck1.size() == 0){
	            				countP1 += war1.size();
	            			}else{
	            				countP2 += war1.size();
	            			}
	            			break;
	            		} else{
	                	  System.out.println("War card for "+ player1.getName() + " is xx\nWar card for " + player2.getName() + " is xx");                     
	                      war1.add(deck1.pop());  //place additional card for war
	                      war1.add(deck2.pop());
	                      
	                      if(deck1.size() > 0 && deck2.size() > 0){
	                    	  p1Card = deck1.pop();
	                    	  p2Card = deck2.pop();
	                      } else{
	                    	  System.out.println("Can't continue War: Deck is out of cards");
	                    	  break;
	                      }
	                  }
	                    
	                    System.out.println("War card for " + player1.getName() + p1Card.toString());
	                    System.out.println("War card for " + player2.getName() + p2Card.toString());
	                    
	                    //if player 1 wins the war round
	                    if(p1Card.getRank() > p2Card.getRank()){
	                        countP1 = countP1 + 2 + war1.size();
	                        System.out.println(player1.getName() +" wins the war round");
	                        break;
	                    }//end if
	                    //otherwise player 2 wins the war round
	                    else if(p2Card.getRank() > p1Card.getRank()){
	                    	countP2 = countP2 + 2 + war1.size();
	                        System.out.println(player2.getName() + " wins the war round");
	                        break;
	                        //nobody won, continue to war again
	                    } else{
	                    	war1.add(p1Card);
	                    	war1.add(p2Card);
	                    	System.out.println("Tie: Continue War");//end else                      
	                    }
	            	}
	                
	            }//end war round else
	            
	            //game over either one player runs out of card(deck size is 0)
	            if(deck1.size() == 0 || deck2.size() == 0){
	              if(countP1 > countP2){
	            	  System.out.println("\nGAME OVER: " + player1.getName() + " wins the game");
	            	  System.out.println(player1.getName() + " Score: "+ countP1+" vs "+ player2.getName() + " Score: "+countP2);
	            	  break;
	              }else if(countP2 > countP1){
	            	  System.out.println("\nGAME OVER: " + player2.getName() + " wins the game");
	            	  System.out.println(player1.getName() + " Score: "+ countP1+" vs "+ player2.getName() + " Score: "+countP2);
	            	  break;
	              }else{
	            	  System.out.println("\nThe game ended in a tie!!");
	            	  System.out.println(player1.getName() + " Score: "+ countP1+" vs "+ player2.getName() + " Score: "+countP2);
	            	  break;
	              }
	            }
	        }//end while  
	}

}
