public abstract class DatabaseSoftware {

    // fields:
    StoreBehavior storeBehavior;

    // constructor
    public  DatabaseSoftware(){
    }

    // Methods
    public void performStore(String newData){
        storeBehavior.store(newData);
    }

    public void setStoreBehavior(StoreBehavior newBehavior){
        storeBehavior = newBehavior;
    }
}
