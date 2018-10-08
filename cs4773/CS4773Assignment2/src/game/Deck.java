package game;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.List;
import java.util.Random;

public class Deck{
	
	List<Card> cards = new ArrayList<>();
	
	public Deck(){
		
		for(int i = 0; i < 4; i++){
			for(int j = 2; j < 15; j++){
				cards.add(new Card(j,i));
			}
		}
		
		Collections.shuffle(cards, new Random());
	}
	
	public int size(){
		return cards.size();
	}
}
