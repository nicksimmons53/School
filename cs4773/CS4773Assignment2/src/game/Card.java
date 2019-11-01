package game;

public class Card {
	private int suit;
	private int rank;
	
	public Card(int rank, int suit) {
		this.rank = rank;
		this.suit = suit;
	}

	public int getRank(){
		return rank;
	}
	
	public void setRank(){
		this.rank = rank;
	}
	
	@Override
	public String toString(){
		
		StringBuilder displayCard = new StringBuilder();
		
		switch(rank){
		case 11:
			displayCard.append("Jack");
			break;
		case 12:
			displayCard.append("Queen");
			break;
		case 13:
			displayCard.append("King");
			break;
		case 14:
			displayCard.append("Ace");
			break;
		default:
			displayCard.append(rank);
			break;
		}
		
		displayCard.append(" of ");
		
		switch(suit){
		case 0:
			displayCard.append("Spades");
			break;
		case 1:
			displayCard.append("Hearts");
			break;
		case 2:
			displayCard.append("Clubs");
			break;
		case 3:
			displayCard.append("Diamonds");
			break;
		}
		
		return displayCard.toString();
	}
	
}
