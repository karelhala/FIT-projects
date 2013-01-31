package server;
/**
 * Trida user slouzi pro praci s uzivatelskym jmenem a heslem
 * @author Libor
 *
 */
public class User {
	/** jmeno */
	private String username;
	/** heslo */
	private String password;
	
	/** Konstruktor */
	public User(String username, String password)
	{
		this.username = username;
		this.password = password;
	}
	/**
	 * Zjisti zda dane jmeno a heslo odpovidaji
	 * @param username jmeno uzivatele
	 * @param password heslo k danemu uzivateli
	 * @return boolean zda se shoduji
	 */
	public boolean isUserNameAndPasswordRight(String username, String password)
	{
		if(this.username.equals(username) && this.password.equals(password))
			return true;
		else
			return false;
	}
	/**
	 * Vrati uzivatelske jmeno
	 * @return username
	 */
    public String getLogin()
    {
    	return this.username;
    }
}
