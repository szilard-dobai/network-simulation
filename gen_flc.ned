//------------ Modulul clockul FLC-ului---------------------
//
// genereaza impulsuri de clock la intrarea FLC
//
simple GeneratorFLC 
{
   	parameters:
        double generateInterval @unit(s);
    gates: 
        output out; 
} 