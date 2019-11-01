package game;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

public class threePlayer_war {
	int p1score = 0;
	int p2score = 0;
	int p3score = 0;
	List<Card> warpile = new ArrayList<>();	
	
	public threePlayer_war(LinkedList<Card> deck1, LinkedList<Card> deck2, LinkedList<Card> deck3) {
		
		while(deck1.size()>0 && deck2.size()>0 && deck3.size()>0) {		
				if(deck1.get(0).getRank() > deck2.get(0).getRank() && deck1.get(0).getRank() > deck3.get(0).getRank()) {
					printThreePlayed(deck1, deck2, deck3);
					System.out.println("player one wins\n");
					addToWarpile(deck1, deck2, deck3);
					p1score+=warpile.size();
					warpile.clear();
				}
				else if(deck2.get(0).getRank() > deck1.get(0).getRank() && deck2.get(0).getRank() > deck3.get(0).getRank()) {
					printThreePlayed(deck1, deck2, deck3);
					System.out.println("player two wins\n");
					addToWarpile(deck1, deck2, deck3);
					p2score+=warpile.size();
					warpile.clear();
				}
				else if(deck3.get(0).getRank() > deck1.get(0).getRank() && deck3.get(0).getRank() > deck2.get(0).getRank()) {
					printThreePlayed(deck1, deck2, deck3);
					System.out.println("player three wins\n");
					addToWarpile(deck1, deck2, deck3);
					p3score+=warpile.size();
					warpile.clear();
				}
				else {
					if(deck1.get(0).getRank()==deck2.get(0).getRank()) {
						List<Card> warpile = new ArrayList<>();
						printThreePlayed(deck1, deck2, deck3);
						int warcase = handleWar(deck1,deck2,warpile,1,2);
						if(warcase == 1) {
							p1score+=warpile.size();
							warpile.clear();
						}
						else if(warcase == 2) {
							p2score+=warpile.size();
							warpile.clear();
						}
					}
					else if(deck1.get(0).getRank()==deck3.get(0).getRank()) {
						List<Card> warpile = new ArrayList<>();
						printThreePlayed(deck1, deck2, deck3);
						int warcase = handleWar(deck1,deck3,warpile,1,3);
						if(warcase == 1) {
							p1score+=warpile.size();
							warpile.clear();
						}
						else if(warcase == 2) {
							p3score+=warpile.size();
							warpile.clear();
						}
					}
					else if(deck3.get(0).getRank()==deck2.get(0).getRank()) {
						List<Card> warpile = new ArrayList<>();
						printThreePlayed(deck1, deck2, deck3);
						int warcase = handleWar(deck3,deck2,warpile,3,2);
						if(warcase == 1) {
							p3score+=warpile.size();
							warpile.clear();
						}
						else if(warcase == 2) {
							p2score+=warpile.size();
							warpile.clear();
						}
					}
					else { // three way war
						handleWarThree(deck1, deck2, deck3, warpile);
					}	
				}
			}
			if(p1score > p2score && p1score > p3score) {
				System.out.println("PLAYER ONE WINS! Score: " +p1score+" VS "+p2score+" VS "+p3score);
			}
			if(p2score > p1score && p2score > p3score) {
				System.out.println("PLAYER TWO WINS! Score: " +p1score+" VS "+p2score+" VS "+p3score);
			}
			if(p3score > p1score && p3score > p2score) {
				System.out.println("PLAYER THREE WINS! Score: " +p1score+" VS "+p2score+" VS "+p3score);
			}
			if(p1score == p2score && p1score == p3score) {
				System.out.println("IT'S A TIE! EVERYONE WINS! Score: " +p1score+" VS "+p2score+" VS "+p3score);
			}		
			if(p1score == p2score && p1score > p3score) {
				System.out.println("Players 1 and 2 TIED!!! PLayer 3 lost!!! Scores : "+p1score+" VS "+p2score+" VS "+p3score);
			}
			if(p1score == p3score && p1score > p2score) {
				System.out.println("Players 1 and 3 TIED!!! PLayer 2 lost!!! Scores : "+p1score+" VS "+p2score+" VS "+p3score);
			}
			if(p2score == p3score && p2score > p1score) {
				System.out.println("Players 2 and 3 TIED!!! PLayer 1 lost!!! Scores : "+p1score+" VS "+p2score+" VS "+p3score);
			}
		}
	public void printThreePlayed(List<Card> one, List<Card> two, List<Card> three) {
		System.out.println("Player1 has played: "+one.get(0).toString());
		System.out.println("Player2 has played: "+two.get(0).toString());
		System.out.println("Player3 has played: "+three.get(0).toString()+"\n");
	}	
	public void addToWarpile(List<Card> one, List<Card> two, List<Card> three) {
		warpile.add(one.remove(0));
		warpile.add(two.remove(0));
		warpile.add(three.remove(0));
	}
	public int handleWar(List<Card> deck1, List<Card> deck2, List<Card> warpile, int playerNum1, int playerNum2) {
		while(deck1.size() >= 2 && deck2.size() >= 2 && deck1.get(0).getRank() == deck2.get(0).getRank() ) {
			System.out.println("Player"+playerNum1+" has played: "+deck1.get(0).toString());
			System.out.println("Player"+playerNum2+" has played: "+deck2.get(0).toString());
			System.out.println("WAR! ");
			//original match
			warpile.add(deck2.remove(0));
			warpile.add(deck1.remove(0));
			//discard for war
			warpile.add(deck2.remove(0));
			warpile.add(deck1.remove(0));
			//final comparison
		}
		if(deck1.size() == 0 || deck2.size() == 0) {
			System.out.println("Not enough cards to finish war :(");
			return 3;
		}
		else if(deck1.get(0).getRank() > deck2.get(0).getRank()) {
			System.out.println("Player"+playerNum1+" has played: "+deck1.get(0).toString());
			System.out.println("Player"+playerNum2+" has played: "+deck2.get(0).toString());
			System.out.println("Player"+playerNum1+" WINS!!!\n");
			deck1.remove(0);
			deck2.remove(0);
			p1score+=2;
			p1score+=warpile.size();
			return 1;
		}
		else if(deck1.get(0).getRank() < deck2.get(0).getRank()) {
			System.out.println("Player"+playerNum1+" has played: "+deck1.get(0).toString());
			System.out.println("Player"+playerNum2+" has played: "+deck2.get(0).toString());
			System.out.println("Player"+playerNum2+" WINS!!!\n");
			deck1.remove(0);
			deck2.remove(0);
			p2score+=2;
			p2score+=warpile.size();
			return 2;
		}
		else {
			//both cards match but theyre the last cards
			System.out.println("Player"+playerNum1+" has played: "+deck1.get(0).toString());
			System.out.println("Player"+playerNum2+" has played: "+deck2.get(0).toString());
			System.out.println("Not enough cards to finish war");
			deck1.remove(0);
			deck2.remove(0);
			return 3;
		}
	}
		public void handleWarThree(List<Card> one, List<Card> two, List<Card>three, List<Card> warpile) {
			while((one.size() >= 2 || two.size() >= 2 || three.size()>=2) && (one.get(0).getRank() == two.get(0).getRank() && two.get(0).getRank() == three.get(0).getRank() && one.get(0).getRank()== three.get(0).getRank())) {
				printThreePlayed(one,two,three);
				System.out.println("WAR! ");
				//original match
				addToWarpile(one,two,three);
				//discard for war
				addToWarpile(one,two,three);
			}
			if(one.get(0).getRank() > two.get(0).getRank() && one.get(0).getRank() > three.get(0).getRank()) {
				printThreePlayed(one, two, three);
				System.out.println("player one wins\n");
				addToWarpile(one, two, three);
				p1score+=warpile.size();
				warpile.clear();
			}
			else if(two.get(0).getRank() > one.get(0).getRank() && two.get(0).getRank() > three.get(0).getRank()) {
				printThreePlayed(one, two, three);
				System.out.println("player two wins\n");
				addToWarpile(one, two, three);
				p2score+=warpile.size();
				warpile.clear();
			}
			else if(three.get(0).getRank() > one.get(0).getRank() && three.get(0).getRank() > two.get(0).getRank()) {
				printThreePlayed(one, two, three);
				System.out.println("player three wins\n");
				addToWarpile(one,two,three);
				p3score+=warpile.size();
				warpile.clear();
			}
			if(one.get(0).getRank()==two.get(0).getRank()) {
				int warcase = handleWar(one,two,warpile,1,2);
				if(warcase == 1) {
					p1score+=warpile.size();
					warpile.clear();
				}
				else if(warcase == 2) {
					p2score+=warpile.size();
					warpile.clear();
				}
			}
			else if(one.get(0).getRank()==three.get(0).getRank()) {
				int warcase = handleWar(one,three,warpile,1,3);
				if(warcase == 1) {
					p1score+=warpile.size();
					warpile.clear();
				}
				else if(warcase == 2) {
					p3score+=warpile.size();
					warpile.clear();
				}
			}
			else if(three.get(0).getRank()==two.get(0).getRank()) {			
				int warcase = handleWar(three,two,warpile,3,2);
				if(warcase == 1) {
					p3score+=warpile.size();
					warpile.clear();
				}
				else if(warcase == 2) {
					p2score+=warpile.size();
					warpile.clear();
				}
			}
		}
	}

