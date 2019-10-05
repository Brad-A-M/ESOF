public class NoSQLDataBase extends DatabaseSoftware {

    // inherited Fields:
    //      StoreBehavior storeBehavior;


    // Constructor: when this class gets instatiated, the constructor will set
    //  storeBehavior to DocumentStore

    public NoSQLDataBase(){
        storeBehavior = new DocumentStore();
    }

}
