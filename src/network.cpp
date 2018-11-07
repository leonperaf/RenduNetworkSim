#include "network.h"
#include "random.h"
#include <map>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

void Network::resize(const size_t& Newsize)
{
	
	values.resize(Newsize);
	RNG.normal(values);
	
}

bool Network::add_link(const size_t& a, const size_t& b)
{
	// a can't link with himself
	// if values.size = 100 , node goes [0,99]
	if(( a == b) or ( a >= values.size()) or (b >= values.size())
				 or ( a < 0) or (b < 0))
	return false;
		
	
	//if a and b does exist
	if(links.count(a) != 0){
		
		std::multimap<size_t,size_t>::iterator it = links.find(a);
		size_t maximum = degree(a);
		
		for(size_t i(0); i < maximum ; ++i){
			
			if(it->second == b)return false;
			++it;
			}
	}
	
	links.insert (std::make_pair(a,b));
	links.insert (std::make_pair(b,a));
	return true;
	

}
 
size_t Network::random_connect(const double& mean_deg)
{
	size_t numlink(0);
	
	//we erase all the links made before
	for(size_t i(0); i < values.size() ; ++i){
		if(degree(i) > 0)links.erase(i);	
	}
	
	for (size_t i(0); i < values.size() ; ++i){
		
		size_t nodes = RNG.poisson(mean_deg);
		
		for(size_t j(0) ; j < nodes ; ++j){
			if (add_link(i,RNG.uniform_double(0,this->size()))){ 
				++numlink;		
			}
		}
	}
	return numlink;
}


size_t Network::set_values(const std::vector<double>& vect)
{
	size_t numv (0);
	
	for(size_t i(0) ; i < vect.size() ; ++i){
		
		values[i] = vect[i];
		++numv;
	}
	
	return numv;
	
}

size_t Network::size() const
{
	return values.size();	
}

size_t Network::degree(const size_t &_n) const
{
	//if there is no _n key the iterator goes to the end
	
	if ( links.find(_n) == links.end() ) {
		
		return 0;
		
	} else {
		
		return links.count(_n);
	
	}
	
}

double Network::value(const size_t &_n) const
{
	return values[_n];
}

std::vector<double> Network::sorted_values() const
{
	std::vector<double>sorted = values;
	std::sort(sorted.begin(),sorted.end(),std::greater<double>());
	
	return sorted;
}


std::vector<size_t> Network::neighbors(const size_t& _n) const
{
	std::vector<size_t> n;
	
	//Check if there is at leat a neighbor (Exeption??)
	if( degree(_n) > 0 ){
	
		//allow us to use iterator
		std::multimap<size_t,size_t>copy = links;
		std::multimap<size_t,size_t>::iterator it = copy.find(_n);
		size_t deg = degree(_n);
	
		for(size_t i(0); i < deg ; ++i){
		
			n.push_back(it->second);
			++it;
		}
	
		return n;
	
	}
	
	return n;
	
}
