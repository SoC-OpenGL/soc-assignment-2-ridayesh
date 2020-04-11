The Shader Class contains functions to retrieve, compile and link vertex and fragment shaders to a program. It also contains functions to update a 
uniform variable. The following are members of this class:

Shader(string pathToVertexShader,string pathToFragmentShader):retrieves,compiles and generates a linked shader program out of the two shader programs. //constructor
void use() : to use the generated shader program.
void set[...](string NameOfUniform,[...] value):used to initialize or update uniform variable
                                               [...] - can be bool, int, float