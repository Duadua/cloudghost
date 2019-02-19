# CPBR

graduation_project


if(flag == 1) {
	if(x < c.width - 200) { x += 10; }
	else { flag = 2; y += 10; }
}
else if(flag == 2) {
	if(y < c.height - 100) { y += 10; }
	else { flag = 3; x -= 10; }
}
else if(flag == 3) {
	if(x > 0) { x -= 10; }
	else {flag = 4; y -= 10; }
}
else if(flag == 4) {
	if(y > 0) { y -= 10; }
	else { flag = 1; x += 10; }
}