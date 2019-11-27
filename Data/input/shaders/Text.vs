#version 440

void main()
{
    gl_Position = ftransform();
    gl_TexCoord[0] = gl_MultiTexCoord0;
}


//[ ERROR    ] ERROR: 0:5: 'ftransform' : undeclared identifier 
//ERROR: 0:6: 'ftransform' : function name expected 
//ERROR: 0:5: 'assign' :  cannot convert from 'const highp float' to 'Position 4-component vector of highp float'
//ERROR: 0:6: 'gl_TexCoord' : undeclared identifier 
//ERROR: 0:6: 'gl_TexCoord' : left of '[' is not of type array, matrix, or vector 
//ERROR: 0:6: 'gl_MultiTexCoord0' : undeclared identifier 
//ERROR: 0:6: 'assign' : l-value required (cannot modify a const)
