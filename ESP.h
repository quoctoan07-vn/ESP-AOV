#pragma once

#define ARGB(a, r, g, b) 0 | a << 24 | r << 16 | g << 8 | b

float Rainbow() {
	static float x = 0, y = 0;
	static float r = 0, g = 0, b = 0;
	if (y >= 0.0f && y < 255.0f) {
		r = 255.0f;
		g = 0.0f;
		b = x;
	} else if (y >= 255.0f && y < 510.0f) {
		r = 255.0f - x;
		g = 0.0f;
		b = 255.0f;
	} else if (y >= 510.0f && y < 765.0f) {
		r = 0.0f;
		g = x;
		b = 255.0f;
	} else if (y >= 765.0f && y < 1020.0f) {
		r = 0.0f;
		g = 255.0f;
		b = 255.0f - x;
	} else if (y >= 1020.0f && y < 1275.0f) {
		r = x;
		g = 255.0f;
		b = 0.0f;
	} else if (y >= 1275.0f && y < 1530.0f) {
		r = 255.0f;
		g = 255.0f - x;
		b = 0.0f;
	}
	x+= 0.25f; 
	if (x >= 255.0f)
		x = 0.0f;
	y+= 0.25f;
	if (y > 1530.0f)
		y = 0.0f;
	return ARGB(255, (int)r, (int)g, (int)b);
}
//LActorRoot
VInt3 (*get_location)(void *instance);
VInt3 (*get_forward)(void *instance);
void* (*AsHero)(void *instance);
int (*GiveMyEnemyCamp)(void *instance);

//ActorLinker
bool (*IsHostPlayer)(void *instance);
int (*get_objCamp)(void *instance);
bool (*ActorLinker_get_bVisible)(void *instance);
//
bool (*get_IsDeadState)(void *instance);
int (*actorHP)(void *instance);
int (*actorMaxHP)(void *instance);
void *(*get_camera)();
Vector3 (*worldToScreen)(void *cam, Vector3 pos);
void *(*get_transform)(void*inst);
Vector3 (*get_position)(void*inst);
Vector3 getPosition(void *transform) {
    return get_position(get_transform(transform));
}

vector<void*> players;
void clearPlayers() {
    vector<void*> pls;
    for (int i = 0; i < players.size(); i++) {
        if (players[i] != NULL) {
            pls.push_back(players[i]);
        }
    }
    players = pls;
}

void removeEnemyGivenObject(void* enemyObject) {
    for (auto it = players.begin(); it != players.end(); ++it) {
        if (*it == enemyObject) {
            players.erase(it);
            return;
        }
    }
}
    
bool playerFind(void *pl) {
    if (pl != NULL) {
        for (int i = 0; i < players.size(); i++) {
            if (pl == players[i]) return true;
        }
    }
    return false;
}

int dem(int num) {
    int div = 1;
    
    while (num != 0) {
        num = num / 10;
        div = div * 10;
    }
    
    return div;
}

Vector3 VInt2Vector(VInt3 location, VInt3 forward)
{
    float scaleX = (float)(location.x * dem(forward.x) + forward.x) / (1000 * dem(forward.x));
    float scaleY = (float)(location.y * dem(forward.y) + forward.y) / (1000 * dem(forward.y));
    float scaleZ = (float)(location.z * dem(forward.z) + forward.z) / (1000 * dem(forward.z));

    return Vector3(scaleX, scaleY, scaleZ);
}

ImColor GetHealthColor (float health, float maxHp = 100.0f)
{
    float r = std::min(519 * (maxHp - health) / maxHp, 255.0f);
    float g = std::min(510 * health / maxHp, 255.0f); 
    return ImVec4(r/ 255.0f, g / 255.0f, 0.f, 1.0f);
}

bool isOutsideScreen(ImVec2 pos, ImVec2 screen) {
    if (pos.y < 0) {
        return true;
    }
    if (pos.x > screen.x) {
        return true;
    }
    if (pos.y > screen.y) {
        return true;
    }
    return pos.x < 0;
}

Vector3 add(Vector3 v1, Vector3 v2) {
    return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

ImVec2 pushToScreenBorder(ImVec2 Pos, ImVec2 screen, int offset) {
    int x = (int) Pos.x;
    int y = (int) Pos.y;
    
    if (Pos.y < 0) {
        y = -offset;
    }
    
    if (Pos.x > screen.x) {
        x = (int) screen.x + offset;
    }
    
    if (Pos.y > screen.y) {
        y = (int) screen.y + offset;
    }
    
    if (Pos.x < 0) {
        x = -offset;
    }
    return ImVec2(x, y);
}

namespace ESP {
	void drawText(const char *text, float X, float Y, float size, long color) {
		ImGui::GetBackgroundDrawList()->AddText(NULL, size, ImVec2(X, Y), color, text);
	}
	
	void drawLine(float startX, float startY, float stopX, float stopY, float thicc, long color) {
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(startX, startY), ImVec2(stopX, stopY), color, thicc);
	}
	
	void drawBorder(float X, float Y, float width, float height, float thicc, long color) {
		ImGui::GetBackgroundDrawList()->AddRect(ImVec2(X, Y), ImVec2(X + width, Y + height), color, thicc);
	}
	
	void drawBox(float X, float Y, float width, float height, float thicc, long color) {
		ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(X, Y), ImVec2(X + width, Y + height), color, thicc);
	}
	
	void drawCornerBox(int x, int y, int w, int h, float thickness, long color) {
		int iw = w / 4;
		int ih = h / 4;
		
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x, y), ImVec2(x + iw, y), color, thickness);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x, y), ImVec2(x, y + ih), color, thickness);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x + w - 1, y), ImVec2(x + w - 1, y + ih), color, thickness);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x, y + h), ImVec2(x + iw, y + h), color, thickness);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x + w - iw, y + h), ImVec2(x + w, y + h), color, thickness);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x, y + h - ih), ImVec2(x, y + h), color, thickness);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x + w - 1, y + h - ih), ImVec2(x + w - 1, y + h), color, thickness);
	}
    
void Draw3dBox(Vector3 Transform, void* camera, int glWidth, int glHeight, float thickness, ImU32 color)
{
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    Vector3 position2 = add(Transform, Vector3(0.6, 1.6, 0.6));
    Vector3 position3 = add(Transform, Vector3(0.6, 0, 0.6));
    Vector3 position4 = add(Transform, Vector3(-0.5, 0, 0.6));
    Vector3 position5 = add(Transform, Vector3(-0.5, 1.6, 0.6));
    Vector3 position6 = add(add(Transform, Vector3(0.6, 1.6, 0)), Vector3(0, 0, -0.6));
    Vector3 position7 = add(add(Transform, Vector3(0.6, 0, 0)), Vector3(0, 0, -0.6));
    Vector3 position8 = add(add(Transform, Vector3(-0.5, 0, 0)), Vector3(0, 0, -0.6));
    Vector3 position9 = add(add(Transform, Vector3(-0.5, 1.6, 0)), Vector3(0, 0, -0.6));

    Vector3 vector = worldToScreen(camera, position2);
    Vector3 vector2 = worldToScreen(camera, position3);
    Vector3 vector3 = worldToScreen(camera, position4);
    Vector3 vector4 = worldToScreen(camera, position5);
    Vector3 vector5 = worldToScreen(camera, position6);
    Vector3 vector6 = worldToScreen(camera, position7);
    Vector3 vector7 = worldToScreen(camera, position8);
    Vector3 vector8 = worldToScreen(camera, position9);

    if (vector.z > 0 && vector2.z > 0 && vector3.z > 0 && vector4.z > 0 && vector5.z > 0 && vector6.z > 0 && vector7.z > 0 && vector8.z > 0)
    {
        drawList->AddLine(ImVec2((glWidth - (glWidth - vector.x)), (glHeight - vector.y)), ImVec2((glWidth - (glWidth - vector2.x)), (glHeight - vector2.y)), color, thickness);
        drawList->AddLine(ImVec2((glWidth - (glWidth - vector3.x)), (glHeight - vector3.y)), ImVec2((glWidth - (glWidth - vector2.x)), (glHeight - vector2.y)), color, thickness);
        drawList->AddLine(ImVec2((glWidth - (glWidth - vector.x)), (glHeight - vector.y)), ImVec2((glWidth - (glWidth - vector4.x)), (glHeight - vector4.y)), color, thickness);
        drawList->AddLine(ImVec2((glWidth - (glWidth - vector4.x)), (glHeight - vector4.y)), ImVec2((glWidth - (glWidth - vector3.x)), (glHeight - vector3.y)), color, thickness);

        drawList->AddLine(ImVec2((glWidth - (glWidth - vector5.x)), (glHeight - vector5.y)), ImVec2((glWidth - (glWidth - vector6.x)), (glHeight - vector6.y)), color, thickness);
        drawList->AddLine(ImVec2((glWidth - (glWidth - vector7.x)), (glHeight - vector7.y)), ImVec2((glWidth - (glWidth - vector6.x)), (glHeight - vector6.y)), color, thickness);
        drawList->AddLine(ImVec2((glWidth - (glWidth - vector5.x)), (glHeight - vector5.y)), ImVec2((glWidth - (glWidth - vector8.x)), (glHeight - vector8.y)), color, thickness);
        drawList->AddLine(ImVec2((glWidth - (glWidth - vector8.x)), (glHeight - vector8.y)), ImVec2((glWidth - (glWidth - vector7.x)), (glHeight - vector7.y)), color, thickness);
        
        drawList->AddLine(ImVec2((glWidth - (glWidth - vector.x)), (glHeight - vector.y)), ImVec2((glWidth - (glWidth - vector5.x)), (glHeight - vector5.y)), color, thickness);
        drawList->AddLine(ImVec2((glWidth - (glWidth - vector2.x)), (glHeight - vector2.y)), ImVec2((glWidth - (glWidth - vector6.x)), (glHeight - vector6.y)), color, thickness);
        drawList->AddLine(ImVec2((glWidth - (glWidth - vector3.x)), (glHeight - vector3.y)), ImVec2((glWidth - (glWidth - vector7.x)), (glHeight - vector7.y)), color, thickness);
        drawList->AddLine(ImVec2((glWidth - (glWidth - vector4.x)), (glHeight - vector4.y)), ImVec2((glWidth - (glWidth - vector8.x)), (glHeight - vector8.y)), color, thickness);
        }
    }
    
void DrawHealthBar(float currentHealth, float maxHealth, ImVec2 enemyPos, float Width, float Height)
{
    auto barSize = ImVec2(Width, Height);
    auto fillWidth = barSize.x * (currentHealth / maxHealth);
    auto screenPos = ImVec2(enemyPos.x - barSize.x / 2, enemyPos.y + barSize.y / 2); 
    auto drawList = ImGui::GetBackgroundDrawList();
    drawList->AddRectFilled (screenPos, ImVec2(screenPos.x + barSize.x, screenPos.y + barSize.y), IM_COL32 (0, 0, 0, 180));
    drawList->AddRectFilled (screenPos, ImVec2(screenPos.x + fillWidth, screenPos.y + barSize.y), GetHealthColor(currentHealth, maxHealth));
}

void DrawCircleHealth(int currentHealth, int maxHealth, float radius, ImVec2 position) {
    float a_max = ((3.14159265359f * 2.0f));
    ImU32 healthColor = IM_COL32(45, 180, 45, 255);
    if (currentHealth <= (maxHealth * 0.6)) {
        healthColor = IM_COL32(180, 180, 45, 255);
    }
    if (currentHealth < (maxHealth * 0.3)) {
        healthColor = IM_COL32(180, 45, 45, 255);
    }
    ImGui::GetForegroundDrawList()->PathArcTo(position, radius, (-(a_max / 4.0f)) + (a_max / maxHealth) * (maxHealth - currentHealth), a_max - (a_max / 4.0f));
    ImGui::GetForegroundDrawList()->PathStroke(healthColor, ImDrawFlags_None, 4);
    }
};


void DrawHealthMiniMap(int currentHealth, int maxHealth, float radius, ImVec2 position) {
    float a_max = ((3.14159265359f * 2.1f));
	
	ImU32 healthColor = IM_COL32(120, 0, 0, 130);
    
    ImGui::GetForegroundDrawList()->PathArcTo(position, radius, (-(a_max / 4.0f)) + (a_max / 1) * (1 - 1), a_max - (a_max / 4.0f));
    ImGui::GetForegroundDrawList()->PathStroke(healthColor, ImDrawFlags_None, 4);
    
	
	
    healthColor = IM_COL32(230, 0, 0, 130);
    
    ImGui::GetForegroundDrawList()->PathArcTo(position, radius, (-(a_max / 4.0f)) + (a_max / maxHealth) * (maxHealth - currentHealth), a_max - (a_max / 4.0f));
    ImGui::GetForegroundDrawList()->PathStroke(healthColor, ImDrawFlags_None, 4);
    
}


void DrawCircleHealthAlert(ImVec2 position, int health, int max_health, float radius) {
    float a_max = ((3.14159265359f * 2.0f));
    ImU32 healthColor = IM_COL32(45, 180, 45, 255);
    if (health <= (max_health * 0.6)) {
        healthColor = IM_COL32(180, 180, 45, 255);
    }
    if (health < (max_health * 0.3)) {
        healthColor = IM_COL32(180, 45, 45, 255);
    }
    ImGui::GetForegroundDrawList()->PathArcTo(position, radius, (-(a_max / 4.0f)) + (a_max / max_health) * (max_health - health), a_max - (a_max / 4.0f));
    ImGui::GetForegroundDrawList()->PathStroke(healthColor, ImDrawFlags_None, 4);
}
