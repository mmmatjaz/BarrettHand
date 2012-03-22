function togglePorts()

en='';
% en = strcat(en,'text(0.5, 0.5, ''{\bfAvoidance}'',''texmode'',''on'')');

en = strcat(en,'text(0.3, 0.9, ''\fontsize{10}IP: {\bf');
en = strcat(en,get_param(gcb,'IP'));
en = strcat(en,' \newline\rmport: \bf');
en = strcat(en,get_param(gcb,'port'));
en = strcat(en,'}'', ''horizontalAlignment'', ''center'',''texmode'',''on'')');

switch get_param(gcb,'mode262')
   case 'Velocity'
        temp =  'color(''red'')port_label(''input'',1,''\fontsize{12}BH262 {\newline\bfVelocity}'',''texmode'',''on'')';
        en=strcat(en,temp);
        en=strcat(en,' ');
    case 'Position'
        temp =  'color(''blue'') port_label(''input'',1,''\fontsize{12}BH262 {\newline\bfPosition}'',''texmode'',''on'')';
        en=strcat(en,temp);
        en=strcat(en,' ');
    case 'Other'
        temp =  'color(''black'') port_label(''input'',1,''\fontsize{12}BH262 {\newline\bfOther}'',''texmode'',''on'')';
        en=strcat(en,temp);
        en=strcat(en,' ');
    otherwise
        temp =  'color(''black'') port_label(''input'',1,''\fontsize{12}BH262 {\newline\bfOther}'',''texmode'',''on'')';
        en=strcat(en,temp);
        en=strcat(en,' '); 
end

switch get_param(gcb,'mode280');
    case 'Velocity'
        temp = 'color(''red'') port_label(''input'',2,''\fontsize{12}BH280  {\newline\bfVelocity}'',''texmode'',''on'')';
        en=strcat(en,temp);
        en=strcat(en,' ');
    case 'Position'
        temp =  'color(''blue'') port_label(''input'',2,''\fontsize{12}BH280 {\newline\bfPosition}'',''texmode'',''on'')';
        en=strcat(en,temp);
        en=strcat(en,' ');
    case 'Other'
        temp =  'color(''black'') port_label(''input'',2,''\fontsize{12}BH280 {\newline\bfOther}'',''texmode'',''on'')';
        en=strcat(en,temp);
        en=strcat(en,' ');
    otherwise
        temp =  'color(''black'') port_label(''input'',2,''\fontsize{12}BH280 {\newline\bfOther}'',''texmode'',''on'')';
        en=strcat(en,temp);
        en=strcat(en,' ');
end

values=get_param(gcb,'MaskValues');
names=get_param(gcb,'MaskNames');
labels={
%     'color(''black'') '
'port_label(''output'',' , ',''Server time'')';
'port_label(''output'',' , ',''\fontsize{11}BH262 {\bfPosition}'',''texmode'',''on'');';
'port_label(''output'',' , ',''BH262 {Velocity}'',''texmode'',''on'');';
'port_label(''output'',' , ',''BH262 {DeltaP}'',''texmode'',''on'');';
'port_label(''output'',' , ',''BH262 {Strain}'',''texmode'',''on'');';
'port_label(''output'',' , ',''BH262 {StrainFltr}'',''texmode'',''on'');';
'port_label(''output'',' , ',''\fontsize{11}BH280 {\bfPosition}'',''texmode'',''on'');';
'port_label(''output'',' , ',''BH280 {Velocity}'',''texmode'',''on'');';
'port_label(''output'',' , ',''BH280 {DeltaP}'',''texmode'',''on'');';
'port_label(''output'',' , ',''BH280 {Strain}'',''texmode'',''on'');';
'port_label(''output'',' , ',''BH280 {StrainFltr}'',''texmode'',''on'');';
'port_label(''output'',' , ',''BH280 {PPS}'',''texmode'',''on'');'
};
x=1;
en=strcat(en, 'color(''black'') ');
for i=6:length(values)
    if strcmp(values{i},'on')
        en=strcat(en,labels{i-5,1});
        en=strcat(en,sprintf('%i',x));
        en=strcat(en,labels{i-5,2});
        en=strcat(en,' ');
        x=x+1;
        
    end
end

% for i=1:length(a)
%     en=strcat(en,a{i});
%     en=strcat(en,' ');
% end



set_param(gcb,'MaskDisplay',en)
