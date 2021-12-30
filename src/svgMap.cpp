#include "svgMap.h"

svgMap* caveMap;

svgMap::svgMap(){

	metricHeight = 120;
	titleHeight  = 50;
	tinyFont     = 8;
	smallFont    = 18;
	largeFont    = 45;
	pathWidth    = 3;

	double x, y, z;
	double minX, minY, minZ, maxX, maxY, maxZ;
	ROOT_STN->maxPos(&minX, &minY, &minZ, &maxX, &maxY, &maxZ);

	mapWidth  = (maxX - minX) * scale;
	mapHeight = (maxY - minY) * scale;
        mapDepth  = (maxZ - minZ) * scale; 

	xoffset = mapWidth  / 10 - minX*scale;
	yoffset = mapHeight / 10 + maxY*scale + titleHeight;
	zoffset = mapDepth  / 10 + maxZ*scale + 1.1*mapHeight + titleHeight;

	pageWidth  = mapWidth *1.2;  
	pageHeight = mapHeight*1.2 + metricHeight + titleHeight + mapDepth*1.2;  
	//pageHeight = mapHeight * 1.2 + metricHeight + titleHeight;  
	pageDepth  = mapDepth * 1.2;  

	numStn = ROOT_STN->numStn();

	ROOT_SHOT->metrics( &horDist, &vertDist, &numShot );	
}

std::string svgMap::fileHeader(){

	return "<svg\nversion=\"1.1\"\nwidth=\"" 
		+ std::to_string((int)pageWidth) + "\"\nheight=\"" 
		+ std::to_string((int)pageHeight)
		+ "\">\n<g\nid=\""
		+ mapName + "\">\n\n";
 }//fileHeader

std::string svgMap::background(){

	return "\n<!--\nBackground\n-->\n<rect x=\"0\" y=\"0\" width=\"" 
		+ std::to_string(pageWidth)  + "\" height=\"" 
		+ std::to_string(pageHeight) + "\" style=\"fill:white;stroke-width:3;stroke:black\" />\n";
}// background

std::string svgMap::title(){
	return "\n<!--\nMap Title\n-->\n" 
		+ textBlock( pageWidth/2, titleHeight, largeFont, "middle", mapName );
}// title

std::string svgMap::fileFooter(){
	return "\t</g>\n</svg>";
}//fileHeader

std::string svgMap::XYMap(Shot* currShot, std::string colour){

	double x, y, z, dx, dy, dz;
	std::string map;

	map = "\n<!--\nXY Map Paths\n-->\n\n";

	while( currShot != nullptr ){

		currShot->fromStn->readPos(&x, &y, &z);
		currShot->toStn->readPos(&dx, &dy, &dz);

		     map.append( "\t<path\n\t\tid=\"Stn " 
			+ std::to_string( currShot->fromStn->readID() ) + "-"
			+ std::to_string( currShot->toStn->readID() )
			+ "\"\n\t\td=\""
			+ "M"  + std::to_string( xoffset+(x*scale) )
			+ " "  + std::to_string( yoffset+(y*((-1)*scale)) )
			+ " L" + std::to_string( xoffset+dx*scale )
			+ " "  + std::to_string( yoffset+dy*(-1)*scale )
			+ "\"\n\t\tstyle=\"stroke:"
			+ colour
			+ ";stroke-width:"
			+ std::to_string((int)pathWidth) 
			+ "px;\" />\n");
		     //stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1\" />\n");


		currShot = currShot->nextShot;
	}

	return map;
}// XYMap

std::string svgMap::XZMap(Shot* currShot, std::string colour){

	double x, y, z, dx, dy, dz;
	std::string map;

	map = "\n<!--\nXZ Map Paths\n-->\n\n";

	while( currShot != nullptr ){

		currShot->fromStn->readPos(&x, &y, &z);
		currShot->toStn->readPos(&dx, &dy, &dz);

		     map.append( "\t<path\n\t\tid=\"Stn " 
			+ std::to_string( currShot->fromStn->readID() ) + "-"
			+ std::to_string( currShot->toStn->readID() )
			+ "\"\n\t\td=\""
			+ "M"  + std::to_string( xoffset+(x*scale) )
			+ " "  + std::to_string( zoffset+(z*((-1)*scale)) )
			+ " L" + std::to_string( xoffset+dx*scale )
			+ " "  + std::to_string( zoffset+dz*(-1)*scale )
			+ "\"\n\t\tstyle=\"stroke:"
			+ colour
			+ ";stroke-width:"
			+ std::to_string((int)pathWidth) 
			+ "px;\" />\n");
		currShot = currShot->nextShot;
	}

	return map;
}// XZMap

std::string svgMap::YZMap(Shot* currShot, std::string colour){

	double x, y, z, dx, dy, dz;
	std::string map;

	map = "\n<!--\nYZ Map Paths\n-->\n\n";

	while( currShot != nullptr ){

		currShot->fromStn->readPos(&x, &y, &z);
		currShot->toStn->readPos(&dx, &dy, &dz);

		     map.append( "\t<path\n\t\tid=\"Stn " 
			+ std::to_string( currShot->fromStn->readID() ) + "-"
			+ std::to_string( currShot->toStn->readID() )
			+ "\"\n\t\td=\""
			+ "M"  + std::to_string( xoffset+(y*scale) )
			+ " "  + std::to_string( zoffset+(z*((-1)*scale)) )
			+ " L" + std::to_string( xoffset+dy*scale )
			+ " "  + std::to_string( zoffset+dz*(-1)*scale )
			+ "\"\n\t\tstyle=\"stroke:"
			+ colour
			+ ";stroke-width:"
			+ std::to_string((int)pathWidth) 
			+ "px;\" />\n");

		currShot = currShot->nextShot;
	}

	return map;
}// YZMap

std::string svgMap::XYstnMarker(Station* currStn){
	double x, y, z;
	long id;
	std::string markers;
        
	markers = "\n<!--\nXY Station Markers\n-->\n\n";

	while( currStn != nullptr ){
		id = currStn->readID();
		currStn->readPos(&x, &y, &z);

		if( id < 100 ){
		markers.append( "\n<circle cx=\"" 
			+ std::to_string( x*scale + xoffset ) + "\" cy=\"" 
			+ std::to_string( y*(-1)*scale + yoffset ) + "\" r=\"" 
			+ std::to_string( smallFont*0.75 ) + "\" fill=\"red\" />\n"
			+ textBlock( (x*scale + xoffset) , (y*(-1)*scale + yoffset + smallFont/2.5) , smallFont, "middle", std::to_string( id ) ) );
		}else
		markers.append( "\n<ellipse cx=\"" 
			+ std::to_string( x*scale + xoffset ) + "\" cy=\"" 
			+ std::to_string( y*(-1)*scale + yoffset ) + "\" rx=\"" 
			+ std::to_string( smallFont*1.5 ) + "\" ry=\""
			+ std::to_string( smallFont*0.75 ) +
			+ "\" fill=\"red\" />\n"
			+ textBlock( (x*scale + xoffset) , (y*(-1)*scale + yoffset + smallFont/2.5) , smallFont, "middle", std::to_string( id ) ) );

		currStn = currStn->nextStation;
	}// while
	return markers;

}// XYstnMarker

std::string svgMap::XZstnMarker(Station* currStn){
	double x, y, z;
	long id;
	std::string markers;
        
	markers = "\n<!--\nXZ Station Markers\n-->\n\n";

	while( currStn != nullptr ){
		id = currStn->readID();
		currStn->readPos(&x, &y, &z);

		if( id < 100 ){
		markers.append( "\n<circle cx=\"" 
			+ std::to_string( x*scale + xoffset ) + "\" cy=\"" 
			+ std::to_string( z*(-1)*scale + zoffset ) + "\" r=\"" 
			+ std::to_string( smallFont*0.75 ) + "\" fill=\"red\" />\n"
			+ textBlock( (x*scale + xoffset) , (z*(-1)*scale + zoffset + smallFont/2.5) , smallFont, "middle", std::to_string( id ) ) );
		}else
		markers.append( "\n<ellipse cx=\"" 
			+ std::to_string( x*scale + xoffset ) + "\" cy=\"" 
			+ std::to_string( z*(-1)*scale + zoffset ) + "\" rx=\"" 
			+ std::to_string( smallFont*1.5 ) + "\" ry=\""
			+ std::to_string( smallFont*0.75 ) +
			+ "\" fill=\"red\" />\n"
			+ textBlock( (x*scale + xoffset) , (z*(-1)*scale + zoffset + smallFont/2.5) , smallFont, "middle", std::to_string( id ) ) );

		currStn = currStn->nextStation;
	}// while
	return markers;

}// XZstnMarker

std::string svgMap::YZstnMarker(Station* currStn){
	double x, y, z;
	long id;
	std::string markers;
        
	markers = "\n<!--\nYZ Station Markers\n-->\n\n";

	while( currStn != nullptr ){
		id = currStn->readID();
		currStn->readPos(&x, &y, &z);

		if( id < 100 ){
		markers.append( "\n<circle cx=\"" 
			+ std::to_string( y*scale + xoffset ) + "\" cy=\"" 
			+ std::to_string( z*(-1)*scale + zoffset ) + "\" r=\"" 
			+ std::to_string( smallFont*0.75 ) + "\" fill=\"red\" />\n"
			+ textBlock( (y*scale + xoffset) , (z*(-1)*scale + zoffset + smallFont/2.5) , smallFont, "middle", std::to_string( id ) ) );
		}else
		markers.append( "\n<ellipse cx=\"" 
			+ std::to_string( y*scale + xoffset ) + "\" cy=\"" 
			+ std::to_string( z*(-1)*scale + zoffset ) + "\" rx=\"" 
			+ std::to_string( smallFont*1.5 ) + "\" ry=\""
			+ std::to_string( smallFont*0.75 ) +
			+ "\" fill=\"red\" />\n"
			+ textBlock( (y*scale + xoffset) , (z*(-1)*scale + zoffset + smallFont/2.5) , smallFont, "middle", std::to_string( id ) ) );

		currStn = currStn->nextStation;
	}// while
	return markers;

}// YZstnMarker

std::string svgMap::metrics(){
	double minX, minY, minZ, maxX, maxY, maxZ;
	ROOT_STN->maxPos(&minX, &minY, &minZ, &maxX, &maxY, &maxZ);

	return "\n<!--\nSurvey Metrics\n-->\n" 
	     + textBlock( smallFont, pageHeight - metricHeight + 0*smallFont, smallFont, "start", 
			     "Number of Stations: " + std::to_string( numStn ) )
             + textBlock( smallFont, pageHeight - metricHeight + 1*smallFont, smallFont, "start",
			     "Number of Shots: " + std::to_string( numShot ) )
             + textBlock( smallFont, pageHeight - metricHeight + 2*smallFont, smallFont, "start",
			     "Surveyed Horizontal Distance: " + std::to_string( (int)horDist ) )
             + textBlock( smallFont, pageHeight - metricHeight + 3*smallFont, smallFont, "start",
			     "Surveyed Vertical Distance: " + std::to_string( (int)vertDist ) )
             + textBlock( smallFont, pageHeight - metricHeight + 4*smallFont, smallFont, "start",
			     "Total Vertical Height: " + std::to_string( (int)(maxZ-minZ) ) )
             + textBlock( smallFont, pageHeight - metricHeight + 5*smallFont, smallFont, "start",
			     "Total Surveyed Distance: " + std::to_string( (int)sqrt( pow(vertDist,2) + pow(horDist,2) ) ) );
}

std::string svgMap::watermark(){
	return "\n<!--\nWatermark\n-->\n"
		+ textBlock( (pageWidth-10), (pageHeight-10), tinyFont, "end", "Created by: PHD - A cave survey tool" );
 
}// watermark

std::string svgMap::textBlock(double x, double y, int size, std::string pos, std::string str){


	return "\n<text text-anchor=\"" + pos + "\"  x=\"" + std::to_string( x ) + "\" y=\"" + std::to_string( y )
		+ "\" font-family=\"Verdana\" font-size=\"" + std::to_string( size ) + "\" fill=\"black\">\n"
		+ str
		+ "\n</text>\n";
}
