def double
gcd(double a, double b)
if (a==b) then return (a)
fi; if (a>b) then
return(gcd(a-b,b)) else return(gcd(a,b-a)) fi; fed; print gcd(21.0e+10,15.0e+10);
print 45; print 2*(gcd(21.0e+10, 28.0e+10) + 6).