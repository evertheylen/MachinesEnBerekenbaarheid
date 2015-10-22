
// Header voor de s_CFG klasse
// In realiteit is de s_CFG een subklasse van de template CFG,
// maar s_CFG zal alles doen wat deze header voorschrijft.

class s_CFG {
public:
	
	// Symbolen in de theorie zijn in de praktijk het type std::string (niet char)
	// Waarom niet chars? omdat we meer dan 256 symbolen willen ondersteunen
	// Maar, er mag geen overlap zijn tussen V en T
	
	// Strings in de theorie (openvolging van symbolen) zijn in de praktijk een vector
	// van symbolen, dus std::vector<std::string>.
	
	// ============== Attributen ==============
	
	// Variables
	std::set<std::string> V;
	
	// Terminals
	std::set<std::string> T;
	
	// Productions
	std::map<std::string, std::set<std::vector<std::string>>> P;
	
	std::string S;
	
	
	// ============== Constructors (input) en output ==============
	
	// lege construct
	CFG();
	
	// volledige constructor, P doe je best apart (met functies die daarvoor dienen)
	CFG(std::set<std::string> _V, std::set<std::string> _T, std::string _S);
	
	// Constructor op basis van xml
	CFG(TiXmlDocument& doc);
	
	// schrijft xml weg naar bestand
	void to_xml(std::string filename);
	
	
	// ============== Hulpmethodes ==============
	
	bool is_epsilon(std::vector<std::string> s);
	// 'epsilon' is eigenlijk de lege string, dus in de praktijk de lege vector.
	
	bool is_variable(std::string symbol);
	
	bool is_terminal(std::string symbol);
	
	std::set<std::vector<std::string>> get_bodies(std::string var);
	
	void set_bodies(std::string var, std::set<std::vector<std::string>> bodies);
	
	void add_production(std::string var, std::vector<std::string> new_body);
	
};
