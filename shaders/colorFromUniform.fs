#version 330 core
out vec4 FragColor;
uniform vec4 ourColor; // we set this variable in our render loop
void main()
{
   FragColor = ourColor;
}