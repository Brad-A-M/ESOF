import java.util.Scanner;

public class Main {
    static Scanner myScanner = new Scanner(System.in);

    public static void main(String[] args){

    // Get user input to select which type of defualt database to create

        String clientChoice="0";

         do {
            System.out.print("Choose a Database type by entering the cooresponding integer:\n" +
                    "[1]    Relational\n" +
                    "[2]    NoSQL\n" +
                    "[3]    Graph\n" +
                    "=> ");
            try {
                clientChoice = myScanner.next();
            } catch (Exception e){
                System.out.println("scanner error");
            }

        }while (!(clientChoice.equals("1") || clientChoice.equals("2") || clientChoice.equals("3")));

         System.out.println("");


     // Create the DB based on client selection

        DatabaseSoftware myDB= null;

         switch (clientChoice){
             case "1":
                 System.out.println("Relational Database Selected");
                 myDB = new RelationalDataBase();
                 //DBActions(myRelDB);
                 //myRelDB.performStore();
                 break;

             case "2":
                 System.out.println("NoSQL Database Selected");
                 myDB = new NoSQLDataBase();

                 //DBActions(myNoSQL);
                 //myNoSQL.performStore();

                 break;

             case "3":
                 System.out.println("Graph Database Selected");
                 myDB = new GraphDatabase();
                 //DBActions(myGraph);

                 break;
         }

     // Get user input for next actions
     //     eg. enter data or change storage strategy

        String DBAction = "0";
        String data;

        do{
            System.out.print("Choose an action by entering an integer:\n" +
                    "[1]    Enter New Data\n" +
                    "[2]    Change Storage Strategy\n" +
                    "[9]    Quit\n" +
                    "=> ");
            DBAction = myScanner.next();

            if (DBAction.equals("1")){
                System.out.println("Enter data: ");
                myScanner.nextLine();
                data = myScanner.nextLine();
                myDB.performStore(data);

            } else if (DBAction.equals("2")) {
                do {
                    System.out.println(("Enter new strategy (NoSQL, Relational, Graph"));
                    data = myScanner.next();
                } while (!(data.equalsIgnoreCase("nosql") ||
                        data.equalsIgnoreCase("relational") || data.equalsIgnoreCase("graph")));

                System.out.println("Changing database strategy to: " + data);

                if (data.equalsIgnoreCase("nosql")) {
                    myDB.setStoreBehavior(new DocumentStore());

                } else if (data.equalsIgnoreCase("relational")) {
                    myDB.setStoreBehavior(new TableStore());

                } else {
                    myDB.setStoreBehavior(new NodeStore());
                }
            } else {
                System.out.println("Exiting...");
            }
        } while (!(DBAction.equals("9")));

    }
}
