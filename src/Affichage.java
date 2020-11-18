
public class Affichage {
		
	public void afficherMessage(String message){
		System.out.print(message);
	}

	public void afficherGrille(String grille, int dimension){
		System.out.println("");
		for(int i=0;i<dimension;i++){
			System.out.print("W ");
		}
		System.out.println("W/B");
		for(int i=0;i<dimension;i++){
			for(int j=0;j<=i;j++){
				System.out.print(" ");
			}
			System.out.print("B ");
			for(int j=0;j<dimension;j++){
				System.out.print(grille.charAt(i*dimension+j)+" ");
			}
			System.out.println("B");
		}
		for(int i=0;i<=dimension;i++){
			System.out.print(" ");
		}
		System.out.print("B/W ");
		for(int i=0;i<=dimension-1;i++){
			System.out.print("W ");
		}
		System.out.println("");
	}
}
