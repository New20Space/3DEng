#include "ECS.h"


void ECS::Add—amera(Transform T,—amera CS) {
	entt::entity C = reg.create();

	reg.emplace<Transform>(C, T);
	reg.emplace<—amera>(C, CS);
}
void ECS::AddObj(Transform T,Mesh M) {
	entt::entity O = reg.create();
	
	reg.emplace<Transform>(O, T);
	reg.emplace<Mesh>(O, M);

}

