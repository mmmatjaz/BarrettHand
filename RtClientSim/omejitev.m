function vector = omejitev( vector, limit )
%SATURATION Summary of this function goes here
%   Detailed explanation goes here
for i=1:length(vector)
    if vector(i)>limit
        vector(i)=limit;
    end
    if vector(i)<-limit
        vector(i)=-limit;
    end
end

end

