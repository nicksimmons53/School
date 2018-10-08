package game;

import java.util.LinkedList;
import java.util.Scanner;

public class Game_Launcher {

	public static void main(String[] args) {
		
		Deck deck = new Deck();
		
		System.out.println("How many players? 2 or 3");
		Scanner reader = new Scanner(System.in);
		int num_players = reader.nextInt();
		
		if(num_players != 2 && num_players != 3){
			System.out.println("You must choose: 2 or 3");
		}
		
		switch(num_players){
		
		case 2:
			System.out.println("Select 1 for Classic or 2 for Score");
			int choice = reader.nextInt();
			
			if(choice == 1){
				System.out.println("Enter player1 name: ");
				String player1_name = reader.next();
				Player player1 = new Player(player1_name);
				System.out.println("Enter player2 name: ");
				String player2_name = reader.next();
				Player player2 = new Player(player2_name);
				LinkedList<Card> deck1 = new LinkedList<Card>(deck.cards.subList(0, 25));
				LinkedList<Card> deck2 = new LinkedList<Card>(deck.cards.subList(25, deck.size()));
				War_game game1 = new War_game(deck1, deck2, player1, player2);
			} else if (choice == 2){
				System.out.println("Enter player1 name: ");
				String player1_name = reader.next();
				Player player1 = new Player(player1_name);
				System.out.println("Enter player2 name: ");
				String player2_name = reader.next();
				Player player2 = new Player(player2_name);
				LinkedList<Card> deck1 = new LinkedList<Card>(deck.cards.subList(0,25));
				LinkedList<Card> deck2 = new LinkedList<Card>(deck.cards.subList(25, deck.size()));
				War_Game_ScoreVar game2 = new War_Game_ScoreVar(deck1,deck2, player1, player2);
			}
			break;
		case 3:
			LinkedList<Card> deck1 = new LinkedList<Card>();
			LinkedList<Card> deck2 = new LinkedList<Card>();
			LinkedList<Card> deck3 = new LinkedList<Card>();
	        
	        deck1.addAll(deck.cards.subList(0, 18));              //26 cards for p1       
	        deck2.addAll(deck.cards.subList(18, 36));
	        deck3.addAll(deck.cards.subList(36, deck.size()));
	        threePlayer_war game3 = new threePlayer_war(deck1, deck2, deck3);
	        break;
	        
		default:
			System.out.println("Error: Run program Again");
		}
	}

}
